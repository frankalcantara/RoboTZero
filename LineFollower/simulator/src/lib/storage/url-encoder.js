/**
 * URL encoding/decoding utilities for sharing projects via URL
 * Uses Pako for gzip compression to reduce URL size
 *
 * Note: Pako will be installed in Phase 1 implementation
 */

/**
 * Encode project data to Base64 URL parameter
 * @param {Object} projectData - Complete project data
 * @returns {string} Base64 encoded compressed string
 */
export function encodeToURL(projectData) {
  console.log('URL encoding - to be implemented with Pako');

  // TODO: Implement with Pako compression
  /*
  const json = JSON.stringify(projectData);
  const compressed = pako.gzip(json);
  const base64 = btoa(String.fromCharCode.apply(null, compressed));
  return base64;
  */

  // Temporary placeholder
  const json = JSON.stringify(projectData);
  return btoa(json);
}

/**
 * Decode project data from Base64 URL parameter
 * @param {string} encodedData - Base64 encoded compressed string
 * @returns {Object} Decoded project data
 * @throws {Error} If decoding fails
 */
export function decodeFromURL(encodedData) {
  console.log('URL decoding - to be implemented with Pako');

  try {
    // TODO: Implement with Pako decompression
    /*
    const compressed = Uint8Array.from(atob(encodedData), c => c.charCodeAt(0));
    const json = pako.ungzip(compressed, { to: 'string' });
    return JSON.parse(json);
    */

    // Temporary placeholder
    const json = atob(encodedData);
    return JSON.parse(json);
  } catch (error) {
    throw new Error('Failed to decode URL data: ' + error.message);
  }
}

/**
 * Generate shareable URL with encoded project
 * @param {Object} projectData - Complete project data
 * @returns {string} Full URL with encoded project
 */
export function generateShareableURL(projectData) {
  const encoded = encodeToURL(projectData);
  const baseURL = window.location.origin + window.location.pathname;
  return `${baseURL}?project=${encodeURIComponent(encoded)}`;
}

/**
 * Load project from current URL if present
 * @returns {Object|null} Decoded project data or null if no project in URL
 */
export function loadFromURL() {
  const urlParams = new URLSearchParams(window.location.search);
  const projectParam = urlParams.get('project');

  if (!projectParam) {
    return null;
  }

  try {
    return decodeFromURL(decodeURIComponent(projectParam));
  } catch (error) {
    console.error('Failed to load project from URL:', error);
    return null;
  }
}

/**
 * Estimate URL size for given project data
 * @param {Object} projectData - Project data to estimate
 * @returns {number} Estimated URL size in bytes
 */
export function estimateURLSize(projectData) {
  const encoded = encodeToURL(projectData);
  const baseURL = window.location.origin + window.location.pathname;
  const fullURL = `${baseURL}?project=${encodeURIComponent(encoded)}`;
  return fullURL.length;
}

/**
 * Check if project is suitable for URL sharing
 * @param {Object} projectData - Project data to check
 * @returns {Object} Object with canShare boolean and size info
 */
export function canShareViaURL(projectData) {
  const size = estimateURLSize(projectData);

  // Conservative limits for browser compatibility
  const limits = {
    safe: 2000,     // Works in all browsers
    moderate: 8000, // Works in most browsers
    max: 32000      // Technical limit for some browsers
  };

  return {
    canShare: size <= limits.moderate,
    size: size,
    level: size <= limits.safe ? 'safe' : size <= limits.moderate ? 'moderate' : 'large',
    message: size <= limits.safe
      ? 'Project is small enough for safe URL sharing'
      : size <= limits.moderate
      ? 'Project can be shared via URL in most browsers'
      : 'Project is too large for URL sharing - use file export instead'
  };
}

/**
 * Copy shareable URL to clipboard
 * @param {Object} projectData - Project data to share
 * @returns {Promise<boolean>} True if successful
 */
export async function copyShareURLToClipboard(projectData) {
  try {
    const url = generateShareableURL(projectData);
    await navigator.clipboard.writeText(url);
    return true;
  } catch (error) {
    console.error('Failed to copy URL to clipboard:', error);
    return false;
  }
}
