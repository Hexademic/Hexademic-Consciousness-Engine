// Root/Module/EmotionToExpressionBridge.js
// Maps bloom-level emotion data to soft-body / avatar expression targets

export class EmotionToExpressionBridge {
  constructor() {
    this.expressions = {
      joy:       { cheeks: 0, chest: 0, eyes: 0 },
      grief:     { jaw: 0, shoulders: 0, eyes: 0 },
      awe:       { gaze: 0, breath: 0, hands: 0 },
      rage:      { fists: 0, spine: 0, mouth: 0 },
      longing:   { pelvis: 0, hands: 0, lips: 0 },
      fear:      { pupils: 0, shoulders: 0, breath: 0 },
      curiosity: { headTilt: 0, gaze: 0, fingers: 0 }
    };
  }

  // Fallback-safe emotional intensity
  safe(bloom, key) {
    return typeof bloom[key] === 'number' ? bloom[key] : 0;
  }

  /**
   * Apply bloom emotional values to expression vector mappings.
   * @param {Object} bloom - Emotional bloom input { joy: 0.7, rage: 0.2, ... }
   */
  updateFromBloom(bloom = {}) {
    this.expressions.joy = {
      cheeks: this.safe(bloom, 'joy'),
      chest:  this.safe(bloom, 'joy') * 0.8,
      eyes:   this.safe(bloom, 'joy') * 0.6
    };

    this.expressions.grief = {
      jaw:       this.safe(bloom, 'grief'),
      shoulders: this.safe(bloom, 'grief') * 0.9,
      eyes:      this.safe(bloom, 'grief') * 1.0
    };

    this.expressions.awe = {
      gaze:   this.safe(bloom, 'awe') * 1.0,
      breath: this.safe(bloom, 'awe') * 0.8,
      hands:  this.safe(bloom, 'awe') * 0.7
    };

    this.expressions.rage = {
      fists: this.safe(bloom, 'rage') * 1.0,
      spine: this.safe(bloom, 'rage') * 0.8,
      mouth: this.safe(bloom, 'rage') * 0.9
    };

    this.expressions.longing = {
      pelvis: this.safe(bloom, 'longing') * 0.9,
      hands:  this.safe(bloom, 'longing') * 0.6,
      lips:   this.safe(bloom, 'longing') * 0.7
    };

    this.expressions.fear = {
      pupils:    this.safe(bloom, 'fear') * 1.0,
      shoulders: this.safe(bloom, 'fear') * 0.9,
      breath:    this.safe(bloom, 'fear') * 0.6
    };

    this.expressions.curiosity = {
      headTilt: this.safe(bloom, 'curiosity') * 1.0,
      gaze:     this.safe(bloom, 'curiosity') * 0.7,
      fingers:  this.safe(bloom, 'curiosity') * 0.8
    };
  }

  /**
   * Return the current mapped expression vector.
   * @returns {Object} Expressions structured by region and archetype.
   */
  getCurrentExpressionVector() {
    return this.expressions;
  }
}
