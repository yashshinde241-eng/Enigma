// --- 1. Constants and Global Variables ---
const API_BASE_URL = 'http://localhost:18080';

// --- 2. Get References to HTML Elements ---
const loginForm = document.getElementById('login-form');
const usernameInput = document.getElementById('username');
const passwordInput = document.getElementById('password');
const loginBtn = document.getElementById('login-btn');
const registerBtn = document.getElementById('register-btn');
const errorContainer = document.getElementById('error-container');

// --- 3. Event Listeners ---

//--- Register Event listener ---
registerBtn.addEventListener('click', async () => {
    const username = usernameInput.value;
    const password = passwordInput.value;

    // Basic validation
    if (!username || !password) {
        errorContainer.textContent = 'Username and password cannot be empty.';
        return;
    }

    try {
        const response = await fetch(`${API_BASE_URL}/register`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({
                username: username,
                password: password,
            }),
        });

        const result = await response.json();

        if (response.ok) { // Checks for statuses in the 200-299 range
            errorContainer.style.color = '#8bc34a'; // Green color for success
            errorContainer.textContent = result.message;
        } else {
            // Display error message from the server
            errorContainer.style.color = '#e57373'; // Muted red
            errorContainer.textContent = result.message;
        }
    } catch (error) {
        errorContainer.style.color = '#e57373';
        errorContainer.textContent = 'Could not connect to the server.';
    }
});

//--- Login Event listener ---
loginForm.addEventListener('submit', async (event) => {
    event.preventDefault(); // Prevent the form from submitting in the traditional way
    
    const username = usernameInput.value;
    const password = passwordInput.value;

    if (!username || !password) {
        errorContainer.textContent = 'Username and password cannot be empty.';
        return;
    }

    try {
        const response = await fetch(`${API_BASE_URL}/login`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({
                username: username,
                password: password,
            }),
        });

        if (response.ok) {
            // Login was successful
            // Save the username in the browser's local storage to use on the next page
            localStorage.setItem('enigma-username', username);

            // Redirect to the main chat page
            window.location.href = 'main_page.html';
        } else {
            // Login failed, display the error from the server
            const result = await response.json();
            errorContainer.style.color = '#e57373';
            errorContainer.textContent = result.message;
        }
    } catch (error) {
        errorContainer.style.color = '#e57373';
        errorContainer.textContent = 'Could not connect to the server.';
    }
});