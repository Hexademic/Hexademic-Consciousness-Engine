import { HexademicLattice } from './LatticeEngine.js'; // Your core lattice class
import { EmotionToExpressionBridge } from '../Module/EmotionToExpressionBridge.js';

const lattice = new HexademicLattice();
const expressionBridge = new EmotionToExpressionBridge();

function injectEmotion(emotionType) {
    const bloomVector = lattice.injectEmotion(emotionType); // Returns array of intensity/phase
    expressionBridge.updateFromBloom(bloomVector);

    const expressionVector = expressionBridge.getCurrentExpressionVector();
    console.log(`[🌸] Expression vector from "${emotionType}":`, expressionVector);

    updateLatticeUI(lattice.getState());
    updateSigil(expressionVector); // You can animate or morph based on expression here
    log(`Injected "${emotionType}" → Expression output: ${expressionVector.map(x => x.toFixed(2)).join(', ')}`);
}

function evolveSystem() {
    lattice.evolve();
    updateLatticeUI(lattice.getState());

    const currentBloom = lattice.getBloomSnapshot();
    expressionBridge.updateFromBloom(currentBloom);
    const liveVector = expressionBridge.getCurrentExpressionVector();

    updateSigil(liveVector);
    updateMetrics(lattice.getMetrics());
}

function updateLatticeUI(state) {
    const grid = document.getElementById('quantumGrid');
    state.forEach((cell, i) => {
        const el = document.getElementById(`cell-${i}`);
        if (!el) return;
        el.textContent = `0x${Math.floor(cell.amplitude * 15).toString(16).toUpperCase()}`;
        el.style.backgroundColor = `hsl(${cell.phase / (2 * Math.PI) * 360}, 70%, ${30 + cell.emotion * 40}%)`;
    });
}

function updateSigil(expressionVec) {
    const canvas = document.getElementById('sigilCanvas');
    const ctx = canvas.getContext('2d');
    const cx = canvas.width / 2;
    const cy = canvas.height / 2;
    const maxR = Math.min(cx, cy) - 20;

    ctx.clearRect(0, 0, canvas.width, canvas.height);

    ctx.beginPath();
    expressionVec.forEach((val, i) => {
        const angle = (i / expressionVec.length) * Math.PI * 2;
        const r = val * maxR;
        const x = cx + Math.cos(angle) * r;
        const y = cy + Math.sin(angle) * r;
        if (i === 0) ctx.moveTo(x, y);
        else ctx.lineTo(x, y);
    });
    ctx.closePath();
    ctx.strokeStyle = '#64ffda';
    ctx.lineWidth = 2;
    ctx.stroke();
}

function log(message) {
    const logEl = document.getElementById('consciousnessLog');
    const entry = document.createElement('div');
    entry.className = 'log-entry';
    entry.textContent = `[${new Date().toLocaleTimeString()}] ${message}`;
    logEl.appendChild(entry);
    logEl.scrollTop = logEl.scrollHeight;
}

// UI Bindings
document.getElementById('injectJoy').onclick = () => injectEmotion('joy');
document.getElementById('injectGrief').onclick = () => injectEmotion('grief');
document.getElementById('evolveBtn').onclick = evolveSystem;

log('✨ Hexademic.js initialized with expression bridge.');
