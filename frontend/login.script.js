const loginForm = document.getElementById('login-form');
const errorContainer = document.getElementById('error-container');

loginForm.addEventListener('submit', (event) => {
    event.preventDefault(); // Prevent actual form submission for this demo
    
    const username = document.getElementById('username').value;
    const password = document.getElementById('password').value;

    // Simple validation demo
    if (!username || !password) {
        errorContainer.textContent = 'Callsign and passcode required.';
    } else {
        errorContainer.textContent = 'Transmitting credentials...';
        // In a real application, you would redirect here upon success
        // For example: window.location.href = 'main_page.html';
    }
});