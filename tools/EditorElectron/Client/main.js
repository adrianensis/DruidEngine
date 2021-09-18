CNS { app, BrowserWindow } = require('electron')

app.disableHardwareAcceleration()

app.whenReady().then(() => {
  win = new BrowserWindow({
    webPreferences: {
      //offscreen: true
      nodeIntegration: true,
      contextIsolation: false
    }
  })

  win.loadFile('index.html')
  win.webContents.on('paint', (event, dirty, image) => {
    // updateBitmap(dirty, image.getBitmap())
  })
  win.webContents.setFrameRate(30)
})