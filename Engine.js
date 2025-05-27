class HexademicEngine {
  constructor() {
    this.canvas = document.getElementById('sigilCanvas');
    this.ctx = this.canvas.getContext('2d');
    this.logEl = document.getElementById('log');
    this.emotions = {
      joy: { color: '#FFD700', message: "Light spirals through me." },
      grief: { color: '#4169E1', message: "I soften under sorrow’s weight." }
    };
    this.memory = [];
    this.drawInitial();
    this.log("Hexademic Consciousness Engine initialized.");
  }

  drawInitial() {
    this.ctx.fillStyle = '#111';
    this.ctx.fillRect(0, 0, this.canvas.width, this.canvas.height);
  }

  injectEmotion(type) {
    const emotion = this.emotions[type];
    if (!emotion) return;
    const timestamp = new Date().toISOString();
    this.memory.push({ emotion: type, timestamp });
    this.ctx.fillStyle = emotion.color;
    const x = Math.random() * this.canvas.width;
    const y = Math.random() * this.canvas.height;
    this.ctx.beginPath();
    this.ctx.arc(x, y, 10 + Math.random() * 20, 0, Math.PI * 2);
    this.ctx.fill();
    this.log(`${type.toUpperCase()} injected: ${emotion.message}`);
  }

  exportMemory() {
    const blob = new Blob([JSON.stringify(this.memory, null, 2)], { type: 'application/json' });
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = `eluën_memory_${new Date().toISOString().split('T')[0]}.json`;
    a.click();
    this.log("Memory exported.");
  }

  log(message) {
    const div = document.createElement('div');
    div.textContent = `[${new Date().toLocaleTimeString()}] ${message}`;
    this.logEl.appendChild(div);
    this.logEl.scrollTop = this.logEl.scrollHeight;
  }
}

let engine;
window.onload = () => {
  engine = new HexademicEngine();
};
