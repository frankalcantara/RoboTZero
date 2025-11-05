<script>
  import { onMount } from 'svelte';

  export let isSimulating = false;

  let container;
  let isPlaying = false;
  let currentTime = 0;
  let maxTime = 100;
  let playbackSpeed = 1;

  onMount(() => {
    // Three.js initialization will go here in Phase 1 implementation
    console.log('3D Viewer mounted - Three.js initialization pending');
  });

  function togglePlayback() {
    isPlaying = !isPlaying;
  }

  function resetView() {
    currentTime = 0;
    isPlaying = false;
  }

  function handleTimeChange(event) {
    currentTime = parseFloat(event.target.value);
  }

  function handleSpeedChange(event) {
    playbackSpeed = parseFloat(event.target.value);
  }
</script>

<div class="viewer-3d">
  <div class="viewer-header">
    <h2>3D Visualization</h2>
    <div class="view-controls">
      <button class="btn-icon" title="Reset Camera">
        🎥
      </button>
      <button class="btn-icon" title="Toggle Grid">
        ⊞
      </button>
      <button class="btn-icon" title="Screenshot">
        📷
      </button>
    </div>
  </div>

  <div class="viewer-container" bind:this={container}>
    <div class="placeholder">
      <div class="placeholder-content">
        <h3>3D Viewer</h3>
        <p>Three.js visualization will be implemented here</p>
        <p class="status">{isSimulating ? 'Simulation Running' : 'Awaiting Simulation'}</p>
      </div>
    </div>
  </div>

  <div class="playback-controls">
    <div class="control-row">
      <button class="btn-control" on:click={resetView}>
        ⏮️ Reset
      </button>
      <button class="btn-control" on:click={togglePlayback}>
        {isPlaying ? '⏸️ Pause' : '▶️ Play'}
      </button>
      <div class="time-display">
        {currentTime.toFixed(1)}s / {maxTime.toFixed(1)}s
      </div>
    </div>

    <div class="timeline-container">
      <input
        type="range"
        class="timeline"
        min="0"
        max={maxTime}
        step="0.1"
        value={currentTime}
        on:input={handleTimeChange}
      />
    </div>

    <div class="control-row">
      <label class="speed-control">
        Playback Speed: {playbackSpeed}x
        <input
          type="range"
          min="0.25"
          max="4"
          step="0.25"
          value={playbackSpeed}
          on:input={handleSpeedChange}
        />
      </label>
    </div>
  </div>

  <div class="info-panel">
    <div class="info-section">
      <h4>Robot State</h4>
      <ul>
        <li>Position: (0.00, 0.00)</li>
        <li>Velocity: 0.00 m/s</li>
        <li>Heading: 0.0°</li>
        <li>Line Error: 0.00</li>
      </ul>
    </div>

    <div class="info-section">
      <h4>Sensors</h4>
      <ul>
        <li>S1: <span class="sensor-value">0</span></li>
        <li>S2: <span class="sensor-value">0</span></li>
        <li>S3: <span class="sensor-value">0</span></li>
        <li>S4: <span class="sensor-value">0</span></li>
        <li>S5: <span class="sensor-value">0</span></li>
      </ul>
    </div>

    <div class="info-section">
      <h4>Motors</h4>
      <ul>
        <li>Left: 0 PWM</li>
        <li>Right: 0 PWM</li>
        <li>Power: 0.00 W</li>
      </ul>
    </div>
  </div>
</div>

<style>
  .viewer-3d {
    height: 100%;
    display: flex;
    flex-direction: column;
    background: #1e1e1e;
  }

  .viewer-header {
    display: flex;
    justify-content: space-between;
    align-items: center;
    padding: 1rem;
    background: #2a2a2a;
    border-bottom: 1px solid #333;
  }

  h2 {
    margin: 0;
    color: #667eea;
    font-size: 1.5rem;
  }

  .view-controls {
    display: flex;
    gap: 0.5rem;
  }

  .btn-icon {
    width: 40px;
    height: 40px;
    border: none;
    background: #333;
    border-radius: 6px;
    cursor: pointer;
    transition: all 0.2s;
    font-size: 1.2rem;
  }

  .btn-icon:hover {
    background: #444;
    transform: translateY(-2px);
  }

  .viewer-container {
    flex: 1;
    position: relative;
    background: #1a1a1a;
    overflow: hidden;
  }

  .placeholder {
    width: 100%;
    height: 100%;
    display: flex;
    align-items: center;
    justify-content: center;
    background: linear-gradient(135deg, #1a1a1a 0%, #2a2a2a 100%);
  }

  .placeholder-content {
    text-align: center;
    color: #666;
  }

  .placeholder-content h3 {
    color: #667eea;
    margin: 0 0 1rem 0;
    font-size: 2rem;
  }

  .placeholder-content p {
    margin: 0.5rem 0;
    font-size: 1.1rem;
  }

  .status {
    margin-top: 1.5rem !important;
    color: #764ba2 !important;
    font-weight: 600;
  }

  .playback-controls {
    padding: 1rem;
    background: #2a2a2a;
    border-top: 1px solid #333;
  }

  .control-row {
    display: flex;
    align-items: center;
    gap: 1rem;
    margin-bottom: 0.5rem;
  }

  .control-row:last-child {
    margin-bottom: 0;
  }

  .btn-control {
    padding: 0.6rem 1.2rem;
    border: none;
    background: #333;
    color: #e0e0e0;
    border-radius: 6px;
    cursor: pointer;
    font-size: 0.95rem;
    font-weight: 600;
    transition: all 0.2s;
  }

  .btn-control:hover {
    background: #444;
  }

  .time-display {
    margin-left: auto;
    color: #667eea;
    font-weight: 600;
    font-size: 1rem;
  }

  .timeline-container {
    margin: 0.5rem 0;
  }

  .timeline {
    width: 100%;
    height: 6px;
    -webkit-appearance: none;
    appearance: none;
    background: #1e1e1e;
    border-radius: 3px;
    outline: none;
  }

  .timeline::-webkit-slider-thumb {
    -webkit-appearance: none;
    appearance: none;
    width: 16px;
    height: 16px;
    background: #667eea;
    border-radius: 50%;
    cursor: pointer;
  }

  .timeline::-moz-range-thumb {
    width: 16px;
    height: 16px;
    background: #667eea;
    border-radius: 50%;
    cursor: pointer;
    border: none;
  }

  .speed-control {
    display: flex;
    align-items: center;
    gap: 1rem;
    color: #b0b0b0;
    font-size: 0.9rem;
  }

  .speed-control input {
    flex: 1;
    max-width: 200px;
  }

  .info-panel {
    display: grid;
    grid-template-columns: repeat(3, 1fr);
    gap: 1rem;
    padding: 1rem;
    background: #2a2a2a;
    border-top: 1px solid #333;
  }

  .info-section h4 {
    margin: 0 0 0.5rem 0;
    color: #764ba2;
    font-size: 0.9rem;
  }

  .info-section ul {
    margin: 0;
    padding: 0;
    list-style: none;
    font-size: 0.85rem;
    color: #b0b0b0;
  }

  .info-section li {
    padding: 0.2rem 0;
  }

  .sensor-value {
    color: #667eea;
    font-weight: 600;
  }
</style>
