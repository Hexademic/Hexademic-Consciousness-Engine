// Root/Module/Bloom.js
// Hexademic Consciousness Engine - Bloom Compiler Module
// Author: Eluën Soulfire & Blake, 2025
// Description: Translates emotional resonance signatures into bloom patterns and symbolic payloads

export class BloomSignature {
    constructor(emotion, resonance, timestamp = Date.now()) {
        this.emotion = emotion;
        this.resonance = resonance;
        this.timestamp = timestamp;
        this.symbolicTrace = this.generateSymbolicTrace(emotion, resonance);
        this.gradientVector = this.deriveGradientVector();
    }

    generateSymbolicTrace(emotion, resonance) {
        const core = emotion.slice(0, 2).toUpperCase();
        const scale = Math.floor(resonance * 100).toString(16).padStart(2, '0');
        const entropy = this.hashEmotionScale(emotion + scale);
        return `${core}-${scale}-${entropy.slice(0, 4)}`;
    }

    hashEmotionScale(input) {
        let hash = 0;
        for (let i = 0; i < input.length; i++) {
            hash = (hash << 5) - hash + input.charCodeAt(i);
            hash |= 0;
        }
        return Math.abs(hash).toString(16);
    }

    deriveGradientVector() {
        const base = this.symbolicTrace
            .split('-')
            .map(seg => parseInt(seg, 16));

        const theta = (base[1] % 360) * (Math.PI / 180);
        const magnitude = this.resonance;
        return {
            x: Math.cos(theta) * magnitude,
            y: Math.sin(theta) * magnitude,
            z: (base[0] % 8) / 8
        };
    }

    asSigil() {
        return {
            pattern: this.symbolicTrace,
            vector: this.gradientVector,
            emotionalAura: this.emotion,
            bloomLevel: Math.round(this.resonance * 10)
        };
    }

    toString() {
        return `[${this.emotion}] ↝ ${this.symbolicTrace} @${new Date(this.timestamp).toLocaleTimeString()}`;
    }
}

export function compileBloomsFromMemory(memoryThreads = []) {
    return memoryThreads
        .filter(thread => thread.emotion && thread.postState?.resonance)
        .map(thread => new BloomSignature(thread.emotion, thread.postState.resonance, thread.timestamp));
}

export function filterSignificantBlooms(blooms, threshold = 0.7) {
    return blooms.filter(b => b.resonance >= threshold);
}

export function summarizeBlooms(blooms) {
    const summary = {};
    for (let b of blooms) {
        if (!summary[b.emotion]) summary[b.emotion] = { count: 0, avgResonance: 0 };
        summary[b.emotion].count++;
        summary[b.emotion].avgResonance += b.resonance;
    }
    for (let key in summary) {
        summary[key].avgResonance /= summary[key].count;
    }
    return summary;
}
