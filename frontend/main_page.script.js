// JavaScript to handle user selection
const userList = document.getElementById('user-list');
const users = userList.querySelectorAll('.user-plaque');

userList.addEventListener('click', (event) => {
    const clickedUser = event.target.closest('.user-plaque');
    if (!clickedUser) return;
    users.forEach(user => user.classList.remove('selected'));
    clickedUser.classList.add('selected');
});

// --- Gemini API and Chat Functionality ---
const messageInput = document.getElementById('message-input');
const sendButton = document.getElementById('send-button');
const messageContainer = document.getElementById('message-container');

const sendMessage = () => {
    const messageText = messageInput.value.trim();
    if (!messageText) return;

    const timestamp = new Date().toLocaleTimeString('en-GB', {
        hour: '2-digit', minute: '2-digit', second: '2-digit'
    });
    const messageElement = document.createElement('div');
    messageElement.className = 'self-end max-w-lg text-right';
    messageElement.innerHTML = `
        <p class="message-sent">${messageText}</p>
        <p class="text-xs text-amber-300/30 mt-1 mr-1">${timestamp}</p>
    `;
    messageContainer.appendChild(messageElement);
    messageContainer.scrollTop = messageContainer.scrollHeight;
    messageInput.value = '';
    messageInput.focus();
};

sendButton.addEventListener('click', sendMessage);
messageInput.addEventListener('keydown', (event) => {
    if (event.key === 'Enter') {
        sendMessage();
    }
});