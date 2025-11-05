/**
 * Project serialization/deserialization utilities
 * Handles conversion between internal state and .lfsim file format
 */

/**
 * Serialize project to .lfsim format
 * @param {Object} project - Project data
 * @param {Object} track - Track data
 * @param {Object} robot - Robot data
 * @param {Object} simulation - Simulation data (optional)
 * @returns {string} JSON string in .lfsim format
 */
export function serializeProject(project, track, robot, simulation = null) {
  const lfsim = {
    version: '1.0',
    metadata: {
      name: project.metadata?.name || 'Untitled Project',
      author: project.metadata?.author || '',
      created: project.metadata?.created || new Date().toISOString(),
      modified: new Date().toISOString()
    },
    track: {
      width: track.width || 1200,
      height: track.height || 700,
      points: track.points || [],
      closed: track.closed || false
    },
    robot: {
      mass: robot.mass,
      wheelbase: robot.wheelbase,
      wheelDiameter: robot.wheelDiameter,
      maxSpeed: robot.maxSpeed,
      sensors: {
        count: robot.sensors.count,
        spacing: robot.sensors.spacing,
        height: robot.sensors.height
      },
      pid: {
        kp: robot.pid.kp,
        ki: robot.pid.ki,
        kd: robot.pid.kd
      },
      environment: {
        temperature: robot.environment.temperature,
        friction: robot.environment.friction,
        gravity: robot.environment.gravity
      }
    }
  };

  if (simulation) {
    lfsim.simulation = {
      parameters: simulation.parameters || {},
      results: simulation.results || {}
    };
  }

  return JSON.stringify(lfsim, null, 2);
}

/**
 * Deserialize .lfsim format to project components
 * @param {string} lfsimString - JSON string in .lfsim format
 * @returns {Object} Object containing project, track, robot, and simulation data
 * @throws {Error} If format is invalid
 */
export function deserializeProject(lfsimString) {
  let lfsim;

  try {
    lfsim = JSON.parse(lfsimString);
  } catch (error) {
    throw new Error('Invalid JSON format');
  }

  // Validate version
  if (!lfsim.version || lfsim.version !== '1.0') {
    throw new Error('Unsupported or missing version');
  }

  // Validate required fields
  if (!lfsim.metadata || !lfsim.track || !lfsim.robot) {
    throw new Error('Missing required fields');
  }

  return {
    project: {
      metadata: {
        name: lfsim.metadata.name,
        author: lfsim.metadata.author || '',
        created: lfsim.metadata.created,
        modified: lfsim.metadata.modified
      },
      version: lfsim.version
    },
    track: {
      width: lfsim.track.width,
      height: lfsim.track.height,
      points: lfsim.track.points,
      closed: lfsim.track.closed
    },
    robot: {
      mass: lfsim.robot.mass,
      wheelbase: lfsim.robot.wheelbase,
      wheelDiameter: lfsim.robot.wheelDiameter,
      maxSpeed: lfsim.robot.maxSpeed,
      sensors: {
        count: lfsim.robot.sensors.count,
        spacing: lfsim.robot.sensors.spacing,
        height: lfsim.robot.sensors.height
      },
      pid: {
        kp: lfsim.robot.pid.kp,
        ki: lfsim.robot.pid.ki,
        kd: lfsim.robot.pid.kd
      },
      environment: {
        temperature: lfsim.robot.environment.temperature,
        friction: lfsim.robot.environment.friction,
        gravity: lfsim.robot.environment.gravity
      }
    },
    simulation: lfsim.simulation || null
  };
}

/**
 * Export project to downloadable .lfsim file
 * @param {Object} project - Project data
 * @param {Object} track - Track data
 * @param {Object} robot - Robot data
 * @param {Object} simulation - Simulation data (optional)
 */
export function exportToFile(project, track, robot, simulation = null) {
  const lfsimString = serializeProject(project, track, robot, simulation);
  const blob = new Blob([lfsimString], { type: 'application/json' });
  const url = URL.createObjectURL(blob);

  const a = document.createElement('a');
  a.href = url;
  a.download = `${project.metadata?.name || 'project'}.lfsim`;
  a.click();

  URL.revokeObjectURL(url);
}

/**
 * Import project from .lfsim file
 * @returns {Promise<Object>} Promise resolving to deserialized project data
 */
export function importFromFile() {
  return new Promise((resolve, reject) => {
    const input = document.createElement('input');
    input.type = 'file';
    input.accept = '.lfsim,.json';

    input.onchange = async (e) => {
      const file = e.target.files[0];
      if (!file) {
        reject(new Error('No file selected'));
        return;
      }

      try {
        const text = await file.text();
        const data = deserializeProject(text);
        resolve(data);
      } catch (error) {
        reject(error);
      }
    };

    input.click();
  });
}

/**
 * Validate project data structure
 * @param {Object} project - Project data to validate
 * @returns {boolean} True if valid
 * @throws {Error} If validation fails
 */
export function validateProject(project) {
  if (!project.metadata || !project.metadata.name) {
    throw new Error('Missing project metadata');
  }

  if (!project.version) {
    throw new Error('Missing project version');
  }

  return true;
}

/**
 * Validate track data structure
 * @param {Object} track - Track data to validate
 * @returns {boolean} True if valid
 * @throws {Error} If validation fails
 */
export function validateTrack(track) {
  if (!Array.isArray(track.points)) {
    throw new Error('Track points must be an array');
  }

  if (track.points.length < 3) {
    throw new Error('Track must have at least 3 points');
  }

  return true;
}

/**
 * Validate robot data structure
 * @param {Object} robot - Robot data to validate
 * @returns {boolean} True if valid
 * @throws {Error} If validation fails
 */
export function validateRobot(robot) {
  const required = ['mass', 'wheelbase', 'wheelDiameter', 'maxSpeed', 'sensors', 'pid', 'environment'];

  for (const field of required) {
    if (robot[field] === undefined) {
      throw new Error(`Missing required robot field: ${field}`);
    }
  }

  if (robot.mass <= 0) {
    throw new Error('Robot mass must be positive');
  }

  if (robot.wheelbase <= 0) {
    throw new Error('Robot wheelbase must be positive');
  }

  return true;
}
