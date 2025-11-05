<script>
  import { trackStore } from '../stores/project.js';

  let canvas;
  let ctx;
  let points = [];
  let isDrawing = false;

  $: if (canvas) {
    ctx = canvas.getContext('2d');
    drawTrack();
  }

  function handleCanvasClick(event) {
    const rect = canvas.getBoundingClientRect();
    const x = event.clientX - rect.left;
    const y = event.clientY - rect.top;

    points = [...points, { x, y }];
    trackStore.update(track => ({
      ...track,
      points: points
    }));

    drawTrack();
  }

  function drawTrack() {
    if (!ctx) return;

    // Clear canvas
    ctx.fillStyle = '#1e1e1e';
    ctx.fillRect(0, 0, canvas.width, canvas.height);

    // Draw grid
    ctx.strokeStyle = '#333';
    ctx.lineWidth = 1;
    for (let i = 0; i < canvas.width; i += 50) {
      ctx.beginPath();
      ctx.moveTo(i, 0);
      ctx.lineTo(i, canvas.height);
      ctx.stroke();
    }
    for (let i = 0; i < canvas.height; i += 50) {
      ctx.beginPath();
      ctx.moveTo(0, i);
      ctx.lineTo(canvas.width, i);
      ctx.stroke();
    }

    if (points.length === 0) return;

    // Draw track path
    ctx.strokeStyle = '#667eea';
    ctx.lineWidth = 3;
    ctx.beginPath();
    ctx.moveTo(points[0].x, points[0].y);

    for (let i = 1; i < points.length; i++) {
      ctx.lineTo(points[i].x, points[i].y);
    }
    ctx.stroke();

    // Draw points
    ctx.fillStyle = '#764ba2';
    points.forEach(point => {
      ctx.beginPath();
      ctx.arc(point.x, point.y, 5, 0, Math.PI * 2);
      ctx.fill();
    });
  }

  function clearTrack() {
    points = [];
    trackStore.update(track => ({
      ...track,
      points: []
    }));
    drawTrack();
  }

  function closeLoop() {
    if (points.length > 2) {
      points = [...points, points[0]];
      trackStore.update(track => ({
        ...track,
        points: points,
        closed: true
      }));
      drawTrack();
    }
  }
</script>

<div class="track-editor">
  <div class="toolbar">
    <h2>Track Editor</h2>
    <div class="controls">
      <button on:click={clearTrack} class="btn-secondary">
        Clear Track
      </button>
      <button on:click={closeLoop} class="btn-primary" disabled={points.length < 3}>
        Close Loop
      </button>
      <span class="info">Points: {points.length}</span>
    </div>
  </div>

  <div class="canvas-container">
    <canvas
      bind:this={canvas}
      width="1200"
      height="700"
      on:click={handleCanvasClick}
    />
    <div class="instructions">
      Click on the canvas to add track points. The track will be connected automatically.
    </div>
  </div>
</div>

<style>
  .track-editor {
    display: flex;
    flex-direction: column;
    height: 100%;
    padding: 1rem;
  }

  .toolbar {
    display: flex;
    justify-content: space-between;
    align-items: center;
    margin-bottom: 1rem;
    padding: 1rem;
    background: #2a2a2a;
    border-radius: 8px;
  }

  h2 {
    margin: 0;
    color: #667eea;
    font-size: 1.5rem;
  }

  .controls {
    display: flex;
    gap: 1rem;
    align-items: center;
  }

  .btn-primary, .btn-secondary {
    padding: 0.6rem 1.2rem;
    border: none;
    border-radius: 6px;
    font-size: 0.9rem;
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

  .btn-secondary:hover {
    background: #444;
  }

  .info {
    color: #b0b0b0;
    font-size: 0.9rem;
  }

  .canvas-container {
    flex: 1;
    display: flex;
    flex-direction: column;
    align-items: center;
    overflow: auto;
  }

  canvas {
    border: 2px solid #333;
    border-radius: 8px;
    cursor: crosshair;
    background: #1e1e1e;
  }

  .instructions {
    margin-top: 1rem;
    padding: 1rem;
    background: #2a2a2a;
    border-radius: 8px;
    color: #b0b0b0;
    text-align: center;
    max-width: 600px;
  }
</style>
