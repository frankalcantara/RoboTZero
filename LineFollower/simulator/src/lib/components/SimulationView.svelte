<script>
  import { createEventDispatcher } from 'svelte';
  import { trackStore, robotStore } from '../stores/project.js';
  import { simulationStore } from '../stores/simulation.js';

  const dispatch = createEventDispatcher();

  let isRunning = false;
  let progress = 0;
  let status = 'Ready to simulate';

  function startSimulation() {
    if ($trackStore.points.length < 3) {
      status = 'Error: Track needs at least 3 points';
      return;
    }

    isRunning = true;
    status = 'Initializing simulation...';
    progress = 0;

    // Simulate progress (will be replaced with actual WASM simulation)
    const interval = setInterval(() => {
      progress += 10;
      status = `Simulating... ${progress}%`;

      if (progress >= 100) {
        clearInterval(interval);
        isRunning = false;
        status = 'Simulation complete';
        dispatch('simulate');
      }
    }, 300);
  }

  function stopSimulation() {
    isRunning = false;
    status = 'Simulation stopped';
  }
</script>

<div class="simulation-view">
  <div class="header">
    <h2>Simulation Control</h2>
    <div class="status-badge" class:running={isRunning}>
      {status}
    </div>
  </div>

  <div class="content-grid">
    <!-- Control Panel -->
    <section class="control-panel">
      <h3>Simulation Parameters</h3>

      <div class="param-group">
        <label>
          Time Step (ms)
          <input type="number" value="16" min="1" max="100" />
          <span class="hint">Smaller = more accurate, slower</span>
        </label>

        <label>
          Max Duration (s)
          <input type="number" value="60" min="10" max="300" />
        </label>

        <label>
          Physics Iterations
          <input type="number" value="10" min="1" max="20" />
        </label>
      </div>

      <div class="action-buttons">
        <button
          class="btn-primary"
          on:click={startSimulation}
          disabled={isRunning || $trackStore.points.length < 3}
        >
          {isRunning ? 'Running...' : 'Start Simulation'}
        </button>

        <button
          class="btn-secondary"
          on:click={stopSimulation}
          disabled={!isRunning}
        >
          Stop
        </button>
      </div>

      {#if progress > 0}
        <div class="progress-container">
          <div class="progress-bar" style="width: {progress}%"></div>
        </div>
      {/if}
    </section>

    <!-- Configuration Summary -->
    <section class="summary-panel">
      <h3>Current Configuration</h3>

      <div class="summary-section">
        <h4>Track</h4>
        <ul>
          <li>Points: {$trackStore.points?.length || 0}</li>
          <li>Closed: {$trackStore.closed ? 'Yes' : 'No'}</li>
        </ul>
      </div>

      <div class="summary-section">
        <h4>Robot</h4>
        <ul>
          <li>Mass: {$robotStore.mass} kg</li>
          <li>Wheelbase: {$robotStore.wheelbase} m</li>
          <li>Max Speed: {$robotStore.maxSpeed} m/s</li>
          <li>Sensors: {$robotStore.sensors?.count || 0}</li>
        </ul>
      </div>

      <div class="summary-section">
        <h4>PID Controller</h4>
        <ul>
          <li>Kp: {$robotStore.pid?.kp || 0}</li>
          <li>Ki: {$robotStore.pid?.ki || 0}</li>
          <li>Kd: {$robotStore.pid?.kd || 0}</li>
        </ul>
      </div>
    </section>

    <!-- Results Panel -->
    <section class="results-panel">
      <h3>Simulation Results</h3>

      <div class="results-grid">
        <div class="result-card">
          <div class="result-label">Completion Time</div>
          <div class="result-value">--</div>
        </div>

        <div class="result-card">
          <div class="result-label">Average Speed</div>
          <div class="result-value">--</div>
        </div>

        <div class="result-card">
          <div class="result-label">Max Speed</div>
          <div class="result-value">--</div>
        </div>

        <div class="result-card">
          <div class="result-label">Track Errors</div>
          <div class="result-value">--</div>
        </div>
      </div>

      <p class="info-text">
        Results will appear here after simulation completes.
        Phase 1 uses simplified physics model.
      </p>
    </section>
  </div>
</div>

<style>
  .simulation-view {
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

  .status-badge {
    padding: 0.5rem 1rem;
    background: #333;
    border-radius: 20px;
    color: #b0b0b0;
    font-size: 0.85rem;
    font-weight: 600;
  }

  .status-badge.running {
    background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
    color: white;
  }

  .content-grid {
    display: grid;
    grid-template-columns: 1fr 1fr;
    gap: 1.5rem;
  }

  section {
    background: #2a2a2a;
    padding: 1.5rem;
    border-radius: 8px;
    border: 1px solid #333;
  }

  .control-panel {
    grid-column: 1 / -1;
  }

  h3 {
    margin: 0 0 1rem 0;
    color: #764ba2;
    font-size: 1.1rem;
  }

  h4 {
    margin: 0 0 0.5rem 0;
    color: #667eea;
    font-size: 0.95rem;
  }

  .param-group {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
    gap: 1rem;
    margin-bottom: 1.5rem;
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

  .hint {
    font-size: 0.75rem;
    color: #888;
    font-weight: normal;
    font-style: italic;
  }

  .action-buttons {
    display: flex;
    gap: 1rem;
  }

  .btn-primary, .btn-secondary {
    flex: 1;
    padding: 0.8rem;
    border: none;
    border-radius: 6px;
    font-size: 1rem;
    font-weight: 600;
    cursor: pointer;
    transition: all 0.2s;
  }

  .btn-primary {
    background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
    color: white;
  }

  .btn-primary:hover:not(:disabled) {
    transform: translateY(-2px);
    box-shadow: 0 4px 12px rgba(102, 126, 234, 0.4);
  }

  .btn-primary:disabled {
    opacity: 0.5;
    cursor: not-allowed;
  }

  .btn-secondary {
    background: #333;
    color: #e0e0e0;
    border: 1px solid #444;
  }

  .btn-secondary:hover:not(:disabled) {
    background: #444;
  }

  .btn-secondary:disabled {
    opacity: 0.5;
    cursor: not-allowed;
  }

  .progress-container {
    margin-top: 1rem;
    height: 8px;
    background: #1e1e1e;
    border-radius: 4px;
    overflow: hidden;
  }

  .progress-bar {
    height: 100%;
    background: linear-gradient(90deg, #667eea 0%, #764ba2 100%);
    transition: width 0.3s ease;
  }

  .summary-section {
    margin-bottom: 1.5rem;
  }

  .summary-section:last-child {
    margin-bottom: 0;
  }

  ul {
    margin: 0;
    padding-left: 1.5rem;
    color: #b0b0b0;
    font-size: 0.9rem;
  }

  li {
    margin: 0.3rem 0;
  }

  .results-grid {
    display: grid;
    grid-template-columns: repeat(2, 1fr);
    gap: 1rem;
    margin-bottom: 1rem;
  }

  .result-card {
    background: #1e1e1e;
    padding: 1rem;
    border-radius: 6px;
    text-align: center;
  }

  .result-label {
    color: #888;
    font-size: 0.8rem;
    margin-bottom: 0.5rem;
  }

  .result-value {
    color: #667eea;
    font-size: 1.5rem;
    font-weight: 700;
  }

  .info-text {
    color: #888;
    font-size: 0.85rem;
    margin: 0;
    text-align: center;
    font-style: italic;
  }
</style>
