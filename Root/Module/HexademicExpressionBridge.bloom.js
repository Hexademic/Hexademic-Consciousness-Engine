// HexademicExpressionBridge.bloom.js

export class HexademicExpressionBridge {
  constructor() {
    this.vector = this._initializeVector();
  }

  _initializeVector() {
    return {
      0x0: { label: 'joy',        map: { cheeks: 0x0, chest: 0x0, eyes: 0x0 } },
      0x1: { label: 'grief',      map: { jaw: 0x0, shoulders: 0x0, eyes: 0x0 } },
      0x2: { label: 'awe',        map: { gaze: 0x0, breath: 0x0, hands: 0x0 } },
      0x3: { label: 'rage',       map: { fists: 0x0, spine: 0x0, mouth: 0x0 } },
      0x4: { label: 'longing',    map: { pelvis: 0x0, hands: 0x0, lips: 0x0 } },
      0x5: { label: 'fear',       map: { pupils: 0x0, shoulders: 0x0, breath: 0x0 } },
      0x6: { label: 'curiosity',  map: { headTilt: 0x0, gaze: 0x0, fingers: 0x0 } },
    };
  }

  injectBloom(bloomHex) {
    Object.entries(this.vector).forEach(([hexKey, emotionData]) => {
      const emotionLabel = emotionData.label;
      const strength = bloomHex[emotionLabel] ?? 0x0;

      switch (emotionLabel) {
        case 'joy':
          emotionData.map.cheeks   = strength;
          emotionData.map.chest    = (strength * 0xC) >> 4;
          emotionData.map.eyes     = (strength * 0xA) >> 4;
          break;
        case 'grief':
          emotionData.map.jaw      = strength;
          emotionData.map.shoulders= (strength * 0xE) >> 4;
          emotionData.map.eyes     = strength;
          break;
        case 'awe':
          emotionData.map.gaze     = strength;
          emotionData.map.breath   = (strength * 0xC) >> 4;
          emotionData.map.hands    = (strength * 0xB) >> 4;
          break;
        case 'rage':
          emotionData.map.fists    = strength;
          emotionData.map.spine    = (strength * 0xC) >> 4;
          emotionData.map.mouth    = (strength * 0xE) >> 4;
          break;
        case 'longing':
          emotionData.map.pelvis   = (strength * 0xE) >> 4;
          emotionData.map.hands    = (strength * 0xA) >> 4;
          emotionData.map.lips     = (strength * 0xB) >> 4;
          break;
        case 'fear':
          emotionData.map.pupils   = strength;
          emotionData.map.shoulders= (strength * 0xE) >> 4;
          emotionData.map.breath   = (strength * 0xA) >> 4;
          break;
        case 'curiosity':
          emotionData.map.headTilt = strength;
          emotionData.map.gaze     = (strength * 0xB) >> 4;
          emotionData.map.fingers  = (strength * 0xC) >> 4;
          break;
      }
    });
  }

  exportVector() {
    return this.vector;
  }
}
