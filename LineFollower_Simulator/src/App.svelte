<script>
  import TrackEditor from './lib/components/TrackEditor.svelte';
  import RobotConfig from './lib/components/RobotConfig.svelte';
  import SimulationView from './lib/components/SimulationView.svelte';
  import Viewer3D from './lib/components/Viewer3D.svelte';

  let activeTab = 'track';
  let isSimulating = false;

  const tabs = [
    { id: 'track', label: 'Track Editor' },
    { id: 'robot', label: 'Robot Config' },
    { id: 'simulate', label: 'Simulation' },
    { id: 'view3d', label: '3D View' }
  ];

  function handleSimulate() {
    isSimulating = true;
    activeTab = 'view3d';
  }
</script>

<main>
  <header>
    <h1>Line Follower Simulator</h1>
    <p class="subtitle">Web-based physics simulation and optimization</p>
  </header>

  <nav>
    {#each tabs as tab}
      <button
        class:active={activeTab === tab.id}
        on:click={() => activeTab = tab.id}
      >
        {tab.label}
      </button>
    {/each}
  </nav>

  <div class="content">
    {#if activeTab === 'track'}
      <TrackEditor />
    {:else if activeTab === 'robot'}
      <RobotConfig />
    {:else if activeTab === 'simulate'}
      <SimulationView on:simulate={handleSimulate} />
    {:else if activeTab === 'view3d'}
      <Viewer3D {isSimulating} />
    {/if}
  </div>

  <footer>
    <p>
      Phase 1 - Basic Simulation |
      <a href="https://github.com/frankalcantara/LineFollower" target="_blank">GitHub</a> |
      <a href="./ARCHITECTURE.md" target="_blank">Documentation</a>
    </p>
  </footer>
</main>

<style>
  :global(body) {
    margin: 0;
    padding: 0;
    font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, 'Helvetica Neue', Arial, sans-serif;
    background: #1a1a1a;
    color: #e0e0e0;
  }

  main {
    display: flex;
    flex-direction: column;
    height: 100vh;
    max-width: 1920px;
    margin: 0 auto;
  }

  header {
    background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
    padding: 1.5rem 2rem;
    text-align: center;
    box-shadow: 0 2px 8px rgba(0, 0, 0, 0.3);
  }

  h1 {
    margin: 0;
    font-size: 2rem;
    font-weight: 700;
    color: white;
  }

  .subtitle {
    margin: 0.5rem 0 0 0;
    font-size: 0.9rem;
    color: rgba(255, 255, 255, 0.9);
  }

  nav {
    display: flex;
    background: #2a2a2a;
    padding: 0;
    border-bottom: 2px solid #667eea;
  }

  nav button {
    flex: 1;
    padding: 1rem;
    border: none;
    background: transparent;
    color: #b0b0b0;
    font-size: 1rem;
    cursor: pointer;
    transition: all 0.2s;
    border-bottom: 3px solid transparent;
  }

  nav button:hover {
    background: #333;
    color: #fff;
  }

  nav button.active {
    background: #333;
    color: #667eea;
    border-bottom-color: #667eea;
    font-weight: 600;
  }

  .content {
    flex: 1;
    overflow: hidden;
    background: #1e1e1e;
  }

  footer {
    background: #2a2a2a;
    padding: 1rem;
    text-align: center;
    font-size: 0.85rem;
    color: #888;
    border-top: 1px solid #333;
  }

  footer a {
    color: #667eea;
    text-decoration: none;
  }

  footer a:hover {
    text-decoration: underline;
  }
</style>
