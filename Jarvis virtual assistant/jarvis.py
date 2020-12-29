import pyttsx3  # pip install pyttsx3
import speech_recognition as sr  # pip install speechRecognition
import datetime
import wikipedia  # pip install wikipedia
import webbrowser   # pip install webbrowser
import os   
import pywhatkit    # pip install pywhatkit
import pyaudio      # pip install pyaudio
import pyjokes      # pip install pyjokes
import wolframalpha    # pip install wolframalpha 
import time
from twilio.rest import Client # pip install twilio


listener = sr.Recognizer()
engine = pyttsx3.init('sapi5')
voices = engine.getProperty('voices')
engine.setProperty('voice', voices[0].id)


def talk(text):
    engine.say(text)
    engine.runAndWait()

def wishMe():
    hour = int(datetime.datetime.now().hour)
    if hour >= 0 and hour < 12:
        talk("Good Morning!")

    elif hour >= 12 and hour < 18:
        talk("Good Afternoon!")

    else:
        talk("Good Evening!")

    talk("I am jarvis Sir. Please tell me how may I help you")


def take_command():
    try:
        with sr.Microphone() as source:
            print('listening...')
            voice = listener.listen(source)
            command = listener.recognize_google(voice,language='en-in')
            command = command.lower()
            if 'alexa' in command:
                command = command.replace('alexa', '')
                print(command)
    except Exception as e:    
        print("Say that again please...")  
        return "None"
    return command



def run_Jarvis():
    command = take_command()
    if 'play' in command:
        song = command.replace('play', '')
        talk('playing' + song)
        pywhatkit.playonyt(song)
    elif 'time' in command:
        time = datetime.datetime.now().strftime('%I:%M %p')
        talk("sir the time is" + time)
    elif 'who is' in command:
        person = command.replace('who is', '')
        info = wikipedia.summary(person, 2)
        talk("According to wikipedia" + info)
    elif 'open youtube' in command:
        talk("openning youtube")
        webbrowser.open("youtube.com")
    elif 'wikipedia' in command:
        talk('Searching Wikipedia...')
        command = query.replace("wikipedia", "")
        results = wikipedia.summary(command, 2)
        talk("According to Wikipedia")
        talk(results)
    elif 'open google' in command:
        talk("opening google")
        webbrowser.open("google.com")
    elif 'girlfriend' in command:
        talk("I am in realtionship with wifi")
    elif 'exit' in command:
        talk("Thanks for giving me your time sir")
        exit()
        return 0
    elif 'who made you' in command or 'who created you' in command:
        talk("I have been created by vicky singh")
    elif'jokes' in command:
        talk(pyjokes.get_joke())

    elif "calculate" in command: 
        query=command
        # make account in wolframalpha and ger api id 
        #api_id ="wolframalpha"
        app_id = "5KK3UK-A7ET3U9P3R"
        client = wolframalpha.Client(app_id)
        indx = query.lower().split().index('calculate') 
        query = query.split()[indx + 1:] 
        res = client.query(' '.join(query)) 
        answer = next(res.results).text 
        talk("The answer is " + answer)

    elif "where is" in command:
            command = command.replace("where is", "")
            location = command
            talk("User asked to Locate")
            talk(location)
            webbrowser.open("https://www.google.com/maps/search/" + location + "")
    else:
        talk('Please say the command again.')

    return 1;

wishMe()
r=1
while r:
    r=run_Jarvis()
