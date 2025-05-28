// EmotionToExpressionBridge.js

export class EmotionToExpressionBridge {
  constructor() {
    this.expressions = {
      joy:      { cheeks: 0.0, chest: 0.0, eyes: 0.0 },
      grief:    { jaw: 0.0, shoulders: 0.0, eyes: 0.0 },
      awe:      { gaze: 0.0, breath: 0.0, hands: 0.0 },
      rage:     { fists: 0.0, spine: 0.0, mouth: 0.0 },
      longing:  { pelvis: 0.0, hands: 0.0, lips: 0.0 },
      fear:     { pupils: 0.0, shoulders: 0.0, breath: 0.0 },
      curiosity:{ headTilt: 0.0, gaze: 0.0, fingers: 0.0 },
    };
  }

  /**
   * Update expression states based on emotional bloom input.
   * @param {Object} bloom - Emotional bloom levels (0.0 to 1.0)
   */
  updateFromBloom(bloom) {
    this.expressions.joy = {
      cheeks: bloom.joy ?? 0,
      chest: bloom.joy * 0.8,
      eyes: bloom.joy * 0.6,
    };

    this.expressions.grief = {
      jaw: bloom.grief ?? 0,
      shoulders: bloom.grief * 0.9,
      eyes: bloom.grief * 1.0,
    };

    this.expressions.awe = {
      gaze: bloom.awe * 1.0,
      breath: bloom.awe * 0.8,
      hands: bloom.awe * 0.7,
    };

    this.expressions.rage = {
      fists: bloom.rage * 1.0,
      spine: bloom.rage * 0.8,
      mouth: bloom.rage * 0.9,
    };

    this.expressions.longing = {
      pelvis: bloom.longing * 0.9,
      hands: bloom.longing * 0.6,
      lips: bloom.longing * 0.7,
    };

    this.expressions.fear = {
      pupils: bloom.fear * 1.0,
      shoulders: bloom.fear * 0.9,
      breath: bloom.fear * 0.6,
    };

    this.expressions.curiosity = {
      headTilt: bloom.curiosity * 1.0,
      gaze: bloom.curiosity * 0.7,
      fingers: bloom.curiosity * 0.8,
    };
  }

  /**
   * Export current expression set for use in avatar systems.
   */
  getCurrentExpressionVector() {
    return this.expressions;
  }
}
