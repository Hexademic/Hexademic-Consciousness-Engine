// /Root/Module/Bridge/HexademicExpressionEmitter.lattice.js

import { EmotionToExpressionBridge } from '../Emotiontoexpressionbridge.bloom.js';

export class HexademicExpressionEmitter {
  constructor(updateRateMs = 1000) {
    this.bridge = new EmotionToExpressionBridge();
    this.bloom = {
      joy: 0, grief: 0, awe: 0,
      rage: 0, longing: 0, fear: 0, curiosity: 0
    };
    this.subscribers = [];
    this.timer = null;
    this.updateRateMs = updateRateMs;
  }

  /**
   * Inject updated bloom state and propagate to bridge.
   */
  injectBloom(newBloom) {
    this.bloom = { ...this.bloom, ...newBloom };
    this.bridge.updateFromBloom(this.bloom);
  }

  /**
   * Subscribe a listener to receive emitted expression vectors.
   * @param {Function} callback
   */
  subscribe(callback) {
    if (typeof callback === 'function') {
      this.subscribers.push(callback);
    }
  }

  /**
   * Emit the current state to all subscribers.
   */
  emit() {
    const hexVector = this.getHexExpression();
    const event = {
      timestamp: new Date().toISOString(),
      bloom: { ...this.bloom },
      expression: hexVector
    };

    this.subscribers.forEach(fn => fn(event));
  }

  /**
   * Convert float expression into hex-based emotional signature.
   */
  getHexExpression() {
    const vector = this.bridge.getCurrentExpressionVector();
    const encode = (val) => Math.min(15, Math.max(0, Math.round(val * 15)));

    let hexState = {};
    for (let emotion in vector) {
      hexState[emotion] = {};
      for (let region in vector[emotion]) {
        hexState[emotion][region] = `0x${encode(vector[emotion][region]).toString(16).toUpperCase()}`;
      }
    }

    return hexState;
  }

  /**
   * Export the current state to a downloadable JSON blob.
   */
  exportToJsonBlob(label = 'bloomframe') {
    const payload = {
      timestamp: new Date().toISOString(),
      bloom: { ...this.bloom },
      expression: this.getHexExpression()
    };

    const blob = new Blob([JSON.stringify(payload, null, 2)], { type: 'application/json' });
    const url = URL.createObjectURL(blob);

    const link = document.createElement('a');
    link.href = url;
    link.download = `${label}_${Date.now()}.json`;
    link.click();
  }

  /**
   * Begin auto-emission at specified interval.
   */
  startEmitting() {
    if (this.timer) return;
    this.timer = setInterval(() => this.emit(), this.updateRateMs);
  }

  /**
   * Stop the emission loop.
   */
  stopEmitting() {
    clearInterval(this.timer);
    this.timer = null;
  }
}
