import React from 'react'; // <-- ADD THIS LINE
import './App.css'

function App() {
  return (
    <main className="enigma-container">

      <aside className="roster-panel">
        <header className="roster-header">ONLINE ROSTER</header>
        <div className="roster-list">
          <div className="roster-user">UserA</div>
          <div className="roster-user active">UserB</div>
          <div className="roster-user">UserC</div>
        </div>
        <footer className="roster-footer">
          Logged in as: <strong>secret_agent</strong>
        </footer>
      </aside>

      <section className="chat-panel">
        <header className="chat-header">TRANSMISSION LOG</header>
        <div className="message-display">
          <div className="message received">Incoming message...</div>
          <div className="message sent">Understood. Proceeding...</div>
        </div>
        <footer className="encoder-panel">
          <div className="encoder-title">ENCODER</div>
          <input type="text" className="message-input" placeholder="Type your message here" />
          <button className="send-button">SEND</button>
        </footer>
      </section>

    </main>
  )
}

export default App