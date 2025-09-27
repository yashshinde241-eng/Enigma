// --- 1. Constants and Global Variables ---
const API_BASE_URL = 'http://localhost:18080';
let currentRecipient = ''; // To keep track of who we are chatting with
let loggedInUser = '';

// --- 2. Get References to HTML Elements ---
const userListContainer = document.getElementById('user-list');
const messageContainer = document.getElementById('message-container');
const messageInput = document.getElementById('message-input');
const sendButton = document.getElementById('send-button');
const currentUserSpan = document.getElementById('current-user');
const logoutBtn = document.getElementById('logout-btn');

// --- 3. Main Setup Function ---
function initialize() {
    // Get the logged-in user from local storage
    loggedInUser = localStorage.getItem('enigma-username');

    if (!loggedInUser) {
        // If no user is logged in, redirect back to the login page
        window.location.href = 'login.html';
        return;
    }

    // Display the current user's name
    currentUserSpan.textContent = loggedInUser;

    // Fetch all users for the roster
    fetchAllUsers();
    
    // Set up event listeners
    addUserEventListeners();
    addLogoutEventListener();
    sendButton.addEventListener('click', sendMessage);
    messageInput.addEventListener('keydown', (event) => {
        if (event.key === 'Enter') sendMessage();
    });

    // Start polling for new messages every 2 seconds
    // setInterval(fetchMessages, 2000);
}

// --- 4. Helper and Core Functions ---
async function fetchAllUsers() {
    try {
        const response = await fetch(`${API_BASE_URL}/users`);
        if (!response.ok) {
            console.error("Failed to fetch users");
            return;
        }

        const usernames = await response.json();
        
        userListContainer.innerHTML = ''; // Clear any placeholder users

        usernames.forEach(username => {
            if (username === loggedInUser) return; // Don't show self in roster

            const userElement = document.createElement('div');
            userElement.className = 'user-plaque rounded-lg p-2 text-center font-enigma';
            userElement.textContent = username;
            
            userListContainer.appendChild(userElement);
        });

    } catch (error) {
        console.error("Error fetching users:", error);
    }
}

function addUserEventListeners() {
    userListContainer.addEventListener('click', (event) => {
        const clickedUser = event.target.closest('.user-plaque');
        if (!clickedUser) return;

        const allUsers = userListContainer.querySelectorAll('.user-plaque');
        allUsers.forEach(user => user.classList.remove('selected'));

        clickedUser.classList.add('selected');
        currentRecipient = clickedUser.textContent;
    });
}

function addLogoutEventListener() {
    logoutBtn.addEventListener('click', async () => {
        try {
            await fetch(`${API_BASE_URL}/logout`, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ username: loggedInUser })
            });
        } catch (error) {
            console.error("Logout failed:", error);
        } finally {
            localStorage.removeItem('enigma-username');
            window.location.href = 'login.html';
        }
    });
}

async function sendMessage() {
    const content = messageInput.value.trim();
    if (!content) return;
    if (!currentRecipient) {
        alert("Please select a user to message.");
        return;
    }

    appendMessage(content, loggedInUser, 'sent');
    messageInput.value = '';

    try {
        await fetch(`${API_BASE_URL}/send`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({
                sender: loggedInUser,
                recipient: currentRecipient,
                content: content
            })
        });
    } catch (error) {
        console.error("Failed to send message:", error);
    }
}

async function fetchMessages() {
    if (!loggedInUser) return;

    try {
        const response = await fetch(`${API_BASE_URL}/getMessages?username=${loggedInUser}`);
        if (!response.ok) return;

        const messages = await response.json();
        messages.forEach(msg => {
            appendMessage(msg.content, msg.from, 'received');
        });
    } catch (error) {
        console.error("Failed to fetch messages:", error);
    }
}

function appendMessage(content, sender, type) {
    const messageWrapper = document.createElement('div');
    const messageElement = document.createElement('p');
    const timestampElement = document.createElement('p');

    messageWrapper.className = type === 'sent' ? 'self-end max-w-lg text-right' : 'self-start max-w-lg';
    messageElement.className = `message-${type}`;
    messageElement.textContent = content;

    timestampElement.className = type === 'sent' ? 'text-xs text-amber-300/30 mt-1 mr-1' : 'text-xs text-green-300/30 mt-1 ml-1';
    timestampElement.textContent = new Date().toLocaleTimeString('en-GB');

    messageWrapper.appendChild(messageElement);
    messageWrapper.appendChild(timestampElement);
    messageContainer.appendChild(messageWrapper);

    messageContainer.scrollTop = messageContainer.scrollHeight;
}

// --- 5. Run the setup function when the page loads ---
initialize();