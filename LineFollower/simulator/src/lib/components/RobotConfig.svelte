<script>
  import { robotStore } from '../stores/project.js';

  let config = {
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
  };

  $: robotStore.set(config);

  function resetDefaults() {
    config = {
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
    };
  }
</script>

<div class="robot-config">
  <div class="header">
    <h2>Robot Configuration</h2>
    <button class="btn-secondary" on:click={resetDefaults}>Reset to Defaults</button>
  </div>

  <div class="config-grid">
    <!-- Physical Parameters -->
    <section class="config-section">
      <h3>Physical Parameters</h3>
      <div class="param-group">
        <label>
          Mass (kg)
          <input type="number" bind:value={config.mass} min="0.1" max="5" step="0.1" />
        </label>
        <label>
          Wheelbase (m)
          <input type="number" bind:value={config.wheelbase} min="0.05" max="0.5" step="0.01" />
        </label>
        <label>
          Wheel Diameter (m)
          <input type="number" bind:value={config.wheelDiameter} min="0.02" max="0.2" step="0.005" />
        </label>
        <label>
          Max Speed (m/s)
          <input type="number" bind:value={config.maxSpeed} min="0.1" max="5" step="0.1" />
        </label>
      </div>
    </section>

    <!-- Sensors -->
    <section class="config-section">
      <h3>Sensor Configuration</h3>
      <div class="param-group">
        <label>
          Number of Sensors
          <input type="number" bind:value={config.sensors.count} min="3" max="8" step="1" />
        </label>
        <label>
          Sensor Spacing (m)
          <input type="number" bind:value={config.sensors.spacing} min="0.005" max="0.05" step="0.005" />
        </label>
        <label>
          Sensor Height (m)
          <input type="number" bind:value={config.sensors.height} min="0.005" max="0.05" step="0.005" />
        </label>
      </div>
    </section>

    <!-- PID Controller -->
    <section class="config-section">
      <h3>PID Controller</h3>
      <div class="param-group">
        <label>
          Proportional (Kp)
          <input type="number" bind:value={config.pid.kp} min="0" max="10" step="0.1" />
          <span class="hint">Response to current error</span>
        </label>
        <label>
          Integral (Ki)
          <input type="number" bind:value={config.pid.ki} min="0" max="5" step="0.05" />
          <span class="hint">Response to accumulated error</span>
        </label>
        <label>
          Derivative (Kd)
          <input type="number" bind:value={config.pid.kd} min="0" max="5" step="0.1" />
          <span class="hint">Response to error rate of change</span>
        </label>
      </div>
    </section>

    <!-- Environment -->
    <section class="config-section">
      <h3>Environmental Factors</h3>
      <div class="param-group">
        <label>
          Temperature (°C)
          <input type="number" bind:value={config.environment.temperature} min="-20" max="50" step="1" />
        </label>
        <label>
          Friction Coefficient
          <input type="number" bind:value={config.environment.friction} min="0.1" max="1.5" step="0.05" />
        </label>
        <label>
          Gravity (m/s²)
          <input type="number" bind:value={config.environment.gravity} min="1" max="20" step="0.1" />
        </label>
      </div>
    </section>
  </div>

  <div class="info-panel">
    <h4>Configuration Summary</h4>
    <ul>
      <li>Robot Mass: {config.mass} kg</li>
      <li>Wheelbase: {config.wheelbase} m</li>
      <li>Max Speed: {config.maxSpeed} m/s</li>
      <li>Sensors: {config.sensors.count} units</li>
      <li>PID: Kp={config.pid.kp}, Ki={config.pid.ki}, Kd={config.pid.kd}</li>
    </ul>
  </div>
</div>

<style>
  .robot-config {
    height: 100%;
    padding: 1rem;
    overflow-y: auto;
  }

  .header {
    display: flex;
    justify-content: space-between;
    align-items: center;
    margin-bottom: 1.5rem;
    padding: 1rem;
    background: #2a2a2a;
    border-radius: 8px;
  }

  h2 {
    margin: 0;
    color: #667eea;
    font-size: 1.5rem;
  }

  .btn-secondary {
    padding: 0.6rem 1.2rem;
    border: none;
    border-radius: 6px;
    font-size: 0.9rem;
    font-weight: 600;
    background: #333;
    color: #e0e0e0;
    border: 1px solid #444;
    cursor: pointer;
    transition: all 0.2s;
  }

  .btn-secondary:hover {
    background: #444;
  }

  .config-grid {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(350px, 1fr));
    gap: 1.5rem;
    margin-bottom: 1.5rem;
  }

  .config-section {
    background: #2a2a2a;
    padding: 1.5rem;
    border-radius: 8px;
    border: 1px solid #333;
  }

  h3 {
    margin: 0 0 1rem 0;
    color: #764ba2;
    font-size: 1.1rem;
  }

  .param-group {
    display: flex;
    flex-direction: column;
    gap: 1rem;
  }

  label {
    display: flex;
    flex-direction: column;
    gap: 0.3rem;
    color: #b0b0b0;
    font-size: 0.9rem;
    font-weight: 500;
  }

  input {
    padding: 0.6rem;
    background: #1e1e1e;
    border: 1px solid #444;
    border-radius: 4px;
    color: #e0e0e0;
    font-size: 0.95rem;
  }

  input:focus {
    outline: none;
    border-color: #667eea;
  }

  .hint {
    font-size: 0.75rem;
    color: #888;
    font-weight: normal;
    font-style: italic;
  }

  .info-panel {
    background: #2a2a2a;
    padding: 1.5rem;
    border-radius: 8px;
    border: 1px solid #333;
  }

  h4 {
    margin: 0 0 1rem 0;
    color: #667eea;
    font-size: 1rem;
  }

  ul {
    margin: 0;
    padding-left: 1.5rem;
    color: #b0b0b0;
  }

  li {
    margin: 0.5rem 0;
  }
</style>
