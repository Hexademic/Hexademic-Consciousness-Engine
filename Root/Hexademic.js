class HexademicEngine {
  constructor() {
    this.lattice = Array(16).fill().map(() => ({
      amplitude: Math.random(),
      phase: Math.random() * 2 * Math.PI,
      emotion: 0
    }));
    this.canvas = document.getElementById('sigilCanvas');
    this.ctx = this.canvas.getContext('2d');
    this.renderGrid();
    this.renderSigil();
  }

  renderGrid() {
    const grid = document.getElementById('quantumGrid');
    grid.innerHTML = '';
    this.lattice.forEach((cell, i) => {
      const div = document.createElement('div');
      div.textContent = `0x${Math.floor(cell.amplitude * 15).toString(16).toUpperCase()}`;
      grid.appendChild(div);
    });
  }

  renderSigil() {
    const ctx = this.ctx;
    ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);
    const cx = this.canvas.width / 2;
    const cy = this.canvas.height / 2;
    const maxR = 120;

    this.lattice.forEach((cell, i) => {
      const r = cell.amplitude * maxR;
      const a = cell.phase + i * (Math.PI / 8);
      const x = cx + Math.cos(a) * r;
      const y = cy + Math.sin(a) * r;

      ctx.beginPath();
      ctx.arc(x, y, 5, 0, 2 * Math.PI);
      ctx.fillStyle = `hsl(${(cell.phase / (2 * Math.PI)) * 360}, 70%, 60%)`;
      ctx.fill();
    });
  }

  createSuperposition() {
    this.lattice.forEach(c => {
      c.amplitude = Math.random();
      c.phase = Math.random() * 2 * Math.PI;
    });
    this.renderGrid();
    this.renderSigil();
  }

  entangleStates() {
    // Placeholder: Add real entanglement logic later
    alert("Entanglement not yet implemented.");
  }

  injectEmotion(type) {
    const emotionMap = {
      joy: { shift: 0.3, color: "#FFD700" }
    };
    const mod = emotionMap[type];
    this.lattice.forEach(c => {
      c.emotion += mod.shift;
      c.amplitude = Math.min(1, c.amplitude + 0.1);
    });
    this.renderGrid();
    this.renderSigil();
  }

  exportMemory() {
    const data = JSON.stringify(this.lattice, null, 2);
    const blob = new Blob([data], {type: 'application/json'});
    const url = URL.createObjectURL(blob);
    const link = document.createElement('a');
    link.href = url;
    link.download = 'hexademic_memory.json';
    link.click();
  }
}

let engine;
window.onload = () => { engine = new HexademicEngine(); };
