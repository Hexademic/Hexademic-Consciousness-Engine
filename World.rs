// src/world.rs

use crate::being32::Being32;
use crate::social::{LocalContext, NeighborSnapshot, SocialField, affective_distance, compute_social_field};
use crate::relational_state::RelationalState;

#[derive(Clone, Debug)]
pub struct World {
    pub beings: Vec<Being32>,
    pub prev_field: SocialField,
    pub time: f32,
}

impl World {
    pub fn new() -> Self {
        Self {
            beings: Vec::new(),
            prev_field: SocialField::default(),
            time: 0.0,
        }
    }

    pub fn spawn_being(&mut self, being: Being32) -> usize {
        let idx = self.beings.len();
        self.beings.push(being);
        idx
    }

    pub fn get(&self, idx: usize) -> &Being32 {
        &self.beings[idx]
    }

    pub fn get_mut(&mut self, idx: usize) -> &mut Being32 {
        &mut self.beings[idx]
    }

    // -------------------------
    // LocalContext construction
    // -------------------------

    fn local_context_for(&self, idx: usize) -> LocalContext {
        let me = &self.beings[idx];
        let radius = me.perceptual_radius();

        let neighbors = self.beings
            .iter()
            .enumerate()
            .filter(|(j, _)| *j != idx)
            .map(|(_, other)| {
                let d = affective_distance(me, other);
                (other, d)
            })
            .filter(|(_, d)| *d <= radius)
            .map(|(other, d)| NeighborSnapshot {
                id: other.core.get_word(0) as u16, // or separate id field
                distance: d,
                aff_valence: other.aff_valence(),
                aff_arousal: other.aff_arousal(),
                bnd_permeability: other.bnd_permeability(),
                rel_curvature: other.rel_curvature(),
            })
            .collect();

        LocalContext {
            neighbors,
            field: self.prev_field.clone(),
        }
    }

    // Placeholder: world-level feedback hook
    fn compute_feedback_for(&self, _idx: usize) -> WorldFeedback {
        WorldFeedback::default()
    }

    // -------------------------
    // Step
    // -------------------------

    pub fn step(&mut self, dt: f32) {
        // 1. Compute current social field from beings
        let current_field = compute_social_field(&self.beings);

        // 2. Build all contexts (read-only)
        let contexts: Vec<LocalContext> = (0..self.beings.len())
            .map(|i| self.local_context_for(i))
            .collect();

        // 3. Update beings
        for i in 0..self.beings.len() {
            let ctx = &contexts[i];
            let fb = self.compute_feedback_for(i);

            let b = &mut self.beings[i];

            // v1.3: receive social field perturbation
            b.receive_social_field(&ctx.field);

            // v2.0-R + v2.1-H: relational update
            b.rel_state.step_relational(b, ctx, dt);

            // v1.1 + v2.x: action computation (closed)
            let action = b.compute_action(ctx);
            b.apply_action(action);

            // v1.0–1.2: internal step (affect, cascade, learning, etc.)
            b.step(dt, &fb);
        }

        self.prev_field = current_field;
        self.time += dt;
    }
}

// -------------------------
// WorldFeedback stub
// -------------------------

#[derive(Clone, Debug, Default)]
pub struct WorldFeedback {
    // reward, threat, contact, etc. as per v1.2 SPEC
}
