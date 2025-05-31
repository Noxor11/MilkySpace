import React from 'react';
import './App.css';
import WasmGameSimple from './components/WasmGameSimple';

function App() {
  return (
    <div className="app-container">
      <header>
        <h1>MilkySpace - WebAssembly Port</h1>
      </header>
      
      <main>
        <WasmGameSimple topScreen={{width: 400, height: 240}} bottomScreen={{width: 320, height: 240}} />
      </main>
      
      <footer>
        <p>Original Nintendo 3DS game ported to WebAssembly </p>
      </footer>
    </div>
  );
}

export default App;