// --- 1. Constants and Global Variables ---
const API_BASE_URL = 'http://192.168.1.45:18080';
let currentRecipient = '';
let loggedInUser = '';
let pollingInterval;
let conversations = {}; // Holds message history, e.g., {'userB': [msg1, msg2]}

// --- 2. Get References to HTML Elements ---
const userListContainer = document.getElementById('user-list');
const messageContainer = document.getElementById('message-container');
const messageInput = document.getElementById('message-input');
const sendButton = document.getElementById('send-button');
const currentUserSpan = document.getElementById('current-user');
const logoutBtn = document.getElementById('logout-btn');

// --- 3. Main Setup Function ---
function initialize() {
    loggedInUser = localStorage.getItem('enigma-username');
    if (!loggedInUser) {
        window.location.href = 'login.html';
        return;
    }
    currentUserSpan.textContent = loggedInUser;
    
    // Initial data fetch
    pollServer(); 
    
    // Set up event listeners
    addUserEventListeners();
    addLogoutEventListener();
    sendButton.addEventListener('click', sendMessage);
    messageInput.addEventListener('keydown', (event) => {
        if (event.key === 'Enter') sendMessage();
    });

    // Start polling for new data every 3 seconds
    pollingInterval = setInterval(pollServer, 3000); 
}

// --- 4. Helper and Core Functions ---

// Combined polling function
async function pollServer() {
    await fetchAllUsers();
    await fetchMessages();
}

async function fetchAllUsers() {
    try {
        const response = await fetch(`${API_BASE_URL}/users`);
        if (!response.ok) return;

        const usernames = await response.json();
        
        const displayedUsernames = new Set([...userListContainer.querySelectorAll('.user-plaque')].map(el => el.textContent));
        const onlineUsernames = new Set(usernames);

        displayedUsernames.forEach(username => {
            if (!onlineUsernames.has(username)) {
                const userToRemove = [...userListContainer.querySelectorAll('.user-plaque')].find(el => el.textContent === username);
                if (userToRemove) userToRemove.remove();
            }
        });

        onlineUsernames.forEach(username => {
            if (!displayedUsernames.has(username) && username !== loggedInUser) {
                const userElement = document.createElement('div');
                userElement.className = 'user-plaque rounded-lg p-2 text-center font-enigma';
                userElement.textContent = username;
                userListContainer.appendChild(userElement);
            }
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

        // Display the conversation for the newly selected user
        displayConversation(currentRecipient);
    });
}

function addLogoutEventListener() {
    logoutBtn.addEventListener('click', async () => {
        clearInterval(pollingInterval);
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
        alert("Please select a user to message first.");
        return;
    }

    appendMessage(content, loggedInUser, 'sent');
    messageInput.value = '';

    try {
        const response = await fetch(`${API_BASE_URL}/send`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({
                sender: loggedInUser,
                recipient: currentRecipient,
                content: content
            })
        });
        if (!response.ok) {
            console.error("Server failed to receive message.");
        }
    } catch (error) {
        console.error("Error sending message:", error);
    }
}

// MODIFIED: Sorts messages into the 'conversations' object
async function fetchMessages() {
    if (!loggedInUser) return;
    try {
        const response = await fetch(`${API_BASE_URL}/getMessages?username=${loggedInUser}`);
        if (!response.ok) return;

        const newMessages = await response.json();
        let shouldUpdateDisplay = false;

        newMessages.forEach(msg => {
            const otherUser = msg.from === loggedInUser ? msg.recipient : msg.from;
            if (!conversations[otherUser]) {
                conversations[otherUser] = [];
            }
            conversations[otherUser].push(msg);

            // If the new message is part of the currently viewed conversation, we should update
            if (otherUser === currentRecipient) {
                shouldUpdateDisplay = true;
            }
        });

        // Re-render the conversation if a new message arrived for it
        if (shouldUpdateDisplay) {
            displayConversation(currentRecipient);
        }

    } catch (error) {
        console.error("Failed to fetch messages:", error);
    }
}

// NEW FUNCTION: Displays messages for a specific user
function displayConversation(username) {
    messageContainer.innerHTML = ''; // Clear the display
    const history = conversations[username] || []; // Get history or an empty array

    history.forEach(msg => {
        const type = msg.from === loggedInUser ? 'sent' : 'received';
        appendMessage(msg.content, msg.from, type);
    });
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