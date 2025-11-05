import { writable, derived } from 'svelte/store';

/**
 * Simulation state store
 */
export const simulationStore = writable({
  isRunning: false,
  isPaused: false,
  progress: 0,
  currentTime: 0,
  maxTime: 100,
  status: 'idle' // idle, running, paused, completed, error
});

/**
 * Simulation results store
 */
export const resultsStore = writable({
  completionTime: null,
  averageSpeed: null,
  maxSpeed: null,
  trackErrors: null,
  trajectory: [],
  energyConsumption: null
});

/**
 * Simulation parameters store
 */
export const simulationParamsStore = writable({
  timeStep: 16, // ms
  maxDuration: 60, // seconds
  physicsIterations: 10,
  playbackSpeed: 1.0
});

/**
 * Robot state during simulation
 */
export const robotStateStore = writable({
  position: { x: 0, y: 0 },
  velocity: { x: 0, y: 0 },
  heading: 0,
  angularVelocity: 0,
  sensors: [0, 0, 0, 0, 0],
  motors: { left: 0, right: 0 },
  lineError: 0,
  power: 0
});

/**
 * Derived store for simulation status text
 */
export const simulationStatusText = derived(
  simulationStore,
  $sim => {
    switch ($sim.status) {
      case 'idle':
        return 'Ready to simulate';
      case 'running':
        return `Simulating... ${$sim.progress}%`;
      case 'paused':
        return 'Simulation paused';
      case 'completed':
        return 'Simulation completed';
      case 'error':
        return 'Simulation error';
      default:
        return 'Unknown status';
    }
  }
);

/**
 * Reset simulation to initial state
 */
export function resetSimulation() {
  simulationStore.set({
    isRunning: false,
    isPaused: false,
    progress: 0,
    currentTime: 0,
    maxTime: 100,
    status: 'idle'
  });

  robotStateStore.set({
    position: { x: 0, y: 0 },
    velocity: { x: 0, y: 0 },
    heading: 0,
    angularVelocity: 0,
    sensors: [0, 0, 0, 0, 0],
    motors: { left: 0, right: 0 },
    lineError: 0,
    power: 0
  });
}
