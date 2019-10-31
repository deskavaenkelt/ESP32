<div>
    <h1>Program ESP32 with Arduino IDE</h1>
    <article>
        <h2>Install Arduino IDE</h2>
        <ul>
            <li>Download latest IDE from <a href="https://www.arduino.cc/en/Main/Software">arduino.cc</a></li>
            <li>Click on "Mac OS X"</li>
            <li>JUST DOWNLOAD</li>
            <li>Move to Applications</li>
            <li>Rightclick Arduino and Open"</li>
        </ul>
    </article>
    <article>
        <h2>Installing ESP32 Add-on in Arduino IDE</h2>
        <h6>Add URL</h6>
        <ul>
            <li>In your Arduino IDE, go to <code>File> Preferences</code></li>
            <li>In “Additional Board Manager URLs” field add <code>https://dl.espressif.com/dl/package_esp32_index.json</code></li>
            <li>NOTE: if you already have the other boards URL, you can separate the URLs with a comma as follows:
                <br>url, url, url</li>
            <li>Click OK</li>
        </ul>
        <h6>Add boards</h6>
        <ul>
            <li>Click <code> Tools > Board > Boards Manager…</code></li>
            <li>Search for ESP32</li>
            <li>Click install button for the “ESP32 by Espressif Systems“ to install new boards</li>
        </ul>
    </article>
    <article>
        <h2>Testing the Installation</h2>
        <ul>
            <li>Select your Board in <code>Tools > Board</code> menu</li>
            <li>LOLIN D32 worked for me</li>
            <li>Select port</li>
        </ul>
    </article>
</div>