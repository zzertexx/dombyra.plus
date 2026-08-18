#pragma once
#include <Arduino.h>

const char INDEX_HTML[] PROGMEM = R"HTMLPAGE(
<!doctype html>
<html lang="kk">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1,viewport-fit=cover">
<meta name="theme-color" content="#f5efe6">
<title>Домбыра</title>
<style>
  :root{
    --bg:#f5efe6; --ink:#1d2b2c; --muted:#7d8a86;
    --teal:#17555c; --teal-d:#0f3d43; --amber:#e0913f; --line:#d9cec0;
    --red:#cf3a2b;
  }
  *{box-sizing:border-box; -webkit-tap-highlight-color:transparent}
  body{margin:0;padding:16px 16px 28px;background:var(--bg);color:var(--ink);
       font-family:-apple-system,BlinkMacSystemFont,"Segoe UI",Roboto,sans-serif}
  h1{font-size:26px;margin:2px 0 0}
  .sub{color:var(--muted);font-size:14px;margin:2px 0 14px}
  .card{background:#fffaf3;border:1px solid var(--line);border-radius:16px;padding:14px;margin-bottom:12px}
  .row{display:flex;gap:10px;align-items:center}
  .grid{display:grid;grid-template-columns:1fr 1fr;gap:10px}
  .fret{text-align:center;padding:10px 4px;border-radius:14px;background:#f0e7da}
  .fret .lbl{font-size:12px;color:var(--muted);letter-spacing:.04em}
  .fret .val{font-size:56px;font-weight:700;line-height:1.05;font-variant-numeric:tabular-nums}
  .fret.s1 .val{color:var(--amber)}
  .fret.s2 .val{color:var(--teal)}
  .fret .nxt{font-size:13px;color:var(--muted)}
  .fret.s1 .val.open, .fret.s2 .val.open{color:var(--red)}   /* 0 = ашық ішек */
  .nxt.open{color:var(--red)}
  .meta{display:flex;justify-content:space-between;font-size:14px;color:var(--muted);margin-top:10px}
  .stroke{font-size:30px;font-weight:700;color:var(--ink)}
  button{font:inherit;font-weight:600;border:0;border-radius:14px;padding:14px 10px;
         background:var(--teal);color:#fff;flex:1;cursor:pointer}
  button:active{background:var(--teal-d)}
  button.ghost{background:#e8ded0;color:var(--teal-d)}
  button.on{background:var(--amber)}
  button:disabled{opacity:.45}
  #next{width:100%;padding:26px;font-size:22px;background:var(--amber);margin-top:4px}
  label{font-size:13px;color:var(--muted);display:flex;justify-content:space-between;margin:12px 0 4px}
  input[type=range]{width:100%;accent-color:var(--teal)}
  .bar{height:12px;border-radius:8px;background:#e8ded0;overflow:hidden;margin-top:8px}
  .bar>i{display:block;height:100%;width:0;background:var(--teal);transition:width .06s linear}
  .bar>i.hit{background:var(--amber)}
  .warn{font-size:13px;color:#9b4a1e;background:#fbe9dc;border-radius:10px;padding:10px;margin-top:10px;display:none}
  .prog{height:6px;border-radius:6px;background:#e8ded0;overflow:hidden;margin-top:12px}
  .prog>i{display:block;height:100%;background:var(--amber);width:0}
</style>
</head>
<body>

<h1>Еркем-ай</h1>
<div class="sub">Қазақтың халық әні · 16 такт</div>

<div class="card">
  <div class="grid">
    <div class="fret s1"><div class="lbl">1-ІШЕК · D18</div><div class="val" id="v1">–</div><div class="nxt" id="n1">&nbsp;</div></div>
    <div class="fret s2"><div class="lbl">2-ІШЕК · D19</div><div class="val" id="v2">–</div><div class="nxt" id="n2">&nbsp;</div></div>
  </div>
  <div class="meta">
    <span id="pos">такт 1/16 · нота 1/56</span>
    <span class="stroke" id="dir">&#8851;</span>
  </div>
  <div class="prog"><i id="prog"></i></div>
</div>

<div class="card">
  <div class="row">
    <button id="play" onclick="cmd('start')">Бастау</button>
    <button class="ghost" onclick="cmd('stop')">Тоқтату</button>
    <button class="ghost" onclick="cmd('restart')">Қайта</button>
  </div>
  <button id="next" onclick="tap()">Келесі нота</button>
</div>

<div class="card">
  <div class="row">
    <button id="mAuto" class="ghost" onclick="cmd('mode','&v=0')">Авто (темп)</button>
    <button id="mMic"  class="ghost" onclick="micMode()">Микрофон</button>
  </div>

  <label><span>Темп</span><b id="bpmV">120 BPM</b></label>
  <input type="range" id="bpm" min="40" max="200" value="120"
         oninput="bpmV.textContent=this.value+' BPM'" onchange="cmd('bpm','&v='+this.value)">

  <label><span>Жарықтық</span><b id="brV">70</b></label>
  <input type="range" id="br" min="5" max="255" value="70"
         oninput="brV.textContent=this.value" onchange="cmd('bright','&v='+this.value)">
</div>

<div class="card" id="micCard">
  <label><span>Микрофон сезімталдығы</span><b id="thV">0.030</b></label>
  <input type="range" id="th" min="3" max="150" value="30"
         oninput="thV.textContent=(this.value/1000).toFixed(3)">
  <div class="bar"><i id="lvl"></i></div>
  <div class="warn" id="micWarn"></div>
</div>

<script>
var S={playing:false,mode:0,bpm:120,bright:70,idx:0,total:56,bar:1,bars:16,
       s1:0,s2:0,dir:68,n1:-1,n2:-1};
var micOn=false, ac=null, an=null, buf=null, env=0, lastTrig=0, busy=0;

function fmt(f){ return f<0 ? '–' : String(f); }

// 0 = ашық ішек (бос қағу) -> қызыл
function setFret(el,f,base,pfx){
  el.innerHTML = (f===undefined||f===null) ? '&nbsp;' : pfx+fmt(f);
  el.className = base + (f===0 ? ' open' : '');
}

function paint(){
  setFret(v1,S.s1,'val','');  setFret(v2,S.s2,'val','');
  setFret(n1,S.n1,'nxt','келесі ');  setFret(n2,S.n2,'nxt','келесі ');
  dir.innerHTML = S.dir===85 ? '&#8744;' : '&#8851;';
  pos.textContent='такт '+S.bar+'/'+S.bars+' · нота '+(S.idx+1)+'/'+S.total;
  prog.style.width = (100*(S.idx+1)/S.total)+'%';
  play.textContent = S.playing ? 'Ойнауда' : 'Бастау';
  play.className = S.playing ? 'on' : '';
  mAuto.className = S.mode===0 ? 'on' : 'ghost';
  mMic.className  = S.mode===1 ? 'on' : 'ghost';
  bpm.value=S.bpm; bpmV.textContent=S.bpm+' BPM';
  br.value=S.bright; brV.textContent=S.bright;
}

async function cmd(c,extra){
  busy++;                       // a command must never be dropped
  try{ var r=await fetch('/api/cmd?c='+c+(extra||''),{cache:'no-store'});
       S=await r.json(); paint(); }catch(e){}
  busy--;
}
async function poll(){
  if(busy) return;
  try{ var r=await fetch('/api/state',{cache:'no-store'}); S=await r.json(); paint(); }catch(e){}
}
setInterval(poll,250);

function tap(){ if(navigator.vibrate) navigator.vibrate(15); cmd('next'); }

// ---- микрофон: әр соққыны естігенде келесі нотаға өтеді -------------------
async function micMode(){
  await cmd('mode','&v=1');
  if(micOn) return;
  if(!navigator.mediaDevices || !navigator.mediaDevices.getUserMedia){
    return warn('Бұл браузер микрофонды HTTP арқылы бермейді. README-дегі нұсқаны қараңыз, немесе «Келесі нота» батырмасын басыңыз.');
  }
  try{
    var st=await navigator.mediaDevices.getUserMedia({audio:{
      echoCancellation:false, noiseSuppression:false, autoGainControl:false}});
    ac=new (window.AudioContext||window.webkitAudioContext)();
    await ac.resume();
    an=ac.createAnalyser(); an.fftSize=1024;
    ac.createMediaStreamSource(st).connect(an);
    buf=new Float32Array(an.fftSize);
    micOn=true; micWarn.style.display='none';
    keepAwake(); listen();
  }catch(e){
    warn('Микрофон қосылмады ('+e.name+'). HTTPS қажет болуы мүмкін — README-ді қараңыз. «Келесі нота» батырмасы әрқашан жұмыс істейді.');
  }
}
function warn(t){ micWarn.textContent=t; micWarn.style.display='block'; }

function listen(){
  requestAnimationFrame(listen);
  if(!micOn) return;
  an.getFloatTimeDomainData(buf);
  var s=0; for(var i=0;i<buf.length;i++) s+=buf[i]*buf[i];
  var rms=Math.sqrt(s/buf.length);

  var thr=th.value/1000;
  var refr=Math.max(90, 30000/S.bpm*0.55);      // ~жарты сегіздік нота
  var now=performance.now();
  var hit = rms>thr && rms>env*1.6 && (now-lastTrig)>refr;

  lvl.style.width=Math.min(100, rms/(thr*3)*100)+'%';
  lvl.className = hit ? 'hit' : '';

  if(hit && S.mode===1 && S.playing){ lastTrig=now; tap(); }
  env=Math.max(rms, env*0.90);
}

async function keepAwake(){ try{ await navigator.wakeLock.request('screen'); }catch(e){} }

poll();
</script>
</body>
</html>
)HTMLPAGE";
