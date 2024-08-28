document.getElementById('annotate-button').addEventListener('click', async () => {
    const inputText = document.getElementById('input-text').value;

    if (inputText.trim() === '') {
        alert('Please enter some text.');
        return;
    }

    showLoadingAnimation();

    try {
        const response = await fetch('http://localhost:5265/api/text', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({ text: inputText })
        });
        
        if (response.status === 202) {
            const requestId = response.headers.get("Location");
            pollForCompletion(requestId);
        } else {
            alert('Unexpected response from the server.');
            hideLoadingAnimation()
        }
    } catch (error) {
        console.error('Error:', error);
        alert('There was an error processing your request.');
    }
});

function pollForCompletion(requestId) {
    setTimeout(async () => {
        try {
            const response = await fetch(`http://localhost:5265/api/text/${requestId}`);
            if(response.status === 200) {
                const responseAsJson = await response.json();
                document.getElementById('output-text').value = responseAsJson.annotatedText;
            } else {
                alert('There was an error processing your request!.');
            }            
        } catch (error) {
            console.error('Error:', error);
            alert('There was an error checking the status of your request.');
        } finally {
            hideLoadingAnimation();
        }
    }, 5000);
}

function showLoadingAnimation() {
    const button = document.getElementById('annotate-button');
    button.disabled = true;
    button.textContent = 'Processing...';
    
    const loader = document.createElement('div');
    loader.className = 'loader';
    button.parentElement.appendChild(loader);
}

function hideLoadingAnimation() {
    const button = document.getElementById('annotate-button');
    button.disabled = false;
    button.textContent = 'Annotate Text';

    const loader = document.querySelector('.loader');
    if (loader) {
        loader.remove();
    }
}
