import { writable } from 'svelte/store';

/**
 * Track store - contains track configuration
 */
export const trackStore = writable({
  points: [],
  closed: false,
  width: 1200,
  height: 700
});

/**
 * Robot store - contains robot configuration
 */
export const robotStore = writable({
  mass: 0.5,
  wheelbase: 0.15,
  wheelDiameter: 0.065,
  maxSpeed: 1.0,
  sensors: {
    count: 5,
    spacing: 0.02,
    height: 0.01
  },
  pid: {
    kp: 1.0,
    ki: 0.1,
    kd: 0.5
  },
  environment: {
    temperature: 25,
    friction: 0.8,
    gravity: 9.81
  }
});

/**
 * Project store - combines track and robot with metadata
 */
export const projectStore = writable({
  metadata: {
    name: 'Untitled Project',
    author: '',
    created: new Date().toISOString(),
    modified: new Date().toISOString()
  },
  version: '1.0'
});

/**
 * Update project modified timestamp
 */
export function updateProjectTimestamp() {
  projectStore.update(project => ({
    ...project,
    metadata: {
      ...project.metadata,
      modified: new Date().toISOString()
    }
  }));
}
