#Name : Jitendra singh
#section : G
#University Roll no.: 2014407
#Project on: voice based mail

#for credentials
from __future__ import print_function
import pickle
import os.path
from googleapiclient.discovery import build
from google_auth_oauthlib.flow import InstalledAppFlow
from google.auth.transport.requests import Request
from google.oauth2.credentials import Credentials

import time
from playsound import  playsound
import datetime
import smtplib #The smtplib is a Python library for sending emails using the Simple Mail Transfer Protocol (SMTP).
import speech_recognition as sr #recogonize voice
import pyttsx3
from email.message import EmailMessage
from word2number import w2n
#1.Turn on the gmail API by creating a new cloud platform project and automatically enable the gmail API.
#2.In resulting dialog click DOWNLOAD CLIENT CONFIGURATION and save the file credentials.jason to your working diirectory.
#3.Install the Google client library  pip 	--upgrade google-api-python-client google-auth-httplib2 google-auth-oauthlib


listener = sr.Recognizer()

engine_start = pyttsx3.init()
voices = engine_start.getProperty('voices')
engine_start.setProperty('voice', voices[1].id)
engine_start.setProperty('rate', 180)

#Read all resources and no write operations
SCOPES = ['https://www.googleapis.com/auth/gmail.readonly']

def main():
    """Shows basic usage of the Gmail API.
    Lists the user's Gmail labels.
    """
    global msg
    global service
    #Variable creds will store the user access token.
    creds = None
    # The file token.json stores the user's access and refresh tokens, and is
    # created automatically when the authorization flow completes for the first
    # time.
    #looking for credentials
    #you are using OAuth 2.0. So there is a basic pattern for accessing a Google API using OAuth 2.0. It follows 4 steps:

    #1.Obtain OAuth 2.0 credentials from the Google Developers Console.
    #2.Obtain an access token from the Google Authorization Server.
    #3.Send the access token to an API.
    #4.Refresh the access token, if necessary.

    if os.path.exists('token.pickle'):
        with open('token.pickle','rb') as token:
            creds=pickle.load(token)
    # If there are no (valid) credentials available, let the user log in.
    if not creds or not creds.valid:
        if creds and creds.expired and creds.refresh_token:
            creds.refresh(Request())
        else:
            flow = InstalledAppFlow.from_client_secrets_file(
                'credentials.json', SCOPES)
            creds = flow.run_local_server(port=0)
         # Save the access token in token.pickle file for the next run
        with open('token.pickle', 'wb') as token:
            pickle.dump(creds,token)
    # Connect to the Gmail API
    service = build('gmail', 'v1', credentials=creds)

    speak("We have some options for you")
    print("1.Unread messages\n2.See messages\n3.send messages\n4.exit")
    speak("speak unread messages to see the unread messages")
    speak("speak see messages to see the message")
    speak("speak send messages for  sending the messages")
    speak("speak exit if you do not want to perform any action")
    choice()
def choice():
        speak("tell me your choice")
        ch=take_information()
        if ch=="unread messages":
            unread()
        elif ch=="see messages":
            see()
        elif ch=="send messages":
            getting_mail_information()
        elif ch=="exit":
            exit(0)
        else:
            speak("please choose from the given option")
            choice()



#Get messages
def unread():
    # request a dictionary of all the messages
    # Added q="is unread" to get just the unread messages
    results= service.users().messages().list(userId='me',labelIds=['INBOX'], q="is:unread").execute()
    messages=results.get('messages',[])
    # messages is a list of dictionaries where each dictionary contains a message id.
    if not messages:
        speak("You have no messages")
        print("You have no messages.")

    else:
        count=0
        for message in messages:
            # Get the message from its id
            msg=service.users().messages().get(userId='me',id=message['id']).execute()
            count=count+1

        print("You have "+str(count)+" unread messages.")
        speak("you have "+str(count)+"unread messages")
        print("would you like to see your messages")
        speak("would you like to see your messages")
        new_choice=take_information()


        if new_choice=="yes":
            #we get the value of payload from the dictionary msg
            emaildata=msg['payload']['headers']
            for values in emaildata:
                #we can get the date ,subject but we only want name from emaildata
                name=values["name"]
                if name=="From":
                    from_name=values["value"]
                    print("You have a new message from : "+from_name)
                    speak("You have a new message from : "+from_name)
                    print("     "+msg['snippet'][:50]+"...")
                    speak("     "+msg['snippet'][:50]+"...")
                    print("\n")
    speak("do you want to perform another action")
    if take_information() =="yes":
        choice()
    else:
        speak('thank you have a nice day')


def see():
    results = service.users().messages().list(userId='me', labelIds=['INBOX']).execute()
    messages = results.get('messages', [])
    print("How many messages you want to see")
    speak("How many messages you want to see")
    count=take_information()
    print(count)
    count1=w2n.word_to_num(count)
    if not messages:
        print('No message found.')
        speak("No message found")
    else:
        print('Messages:')
        for message in messages[:count1]:
            msg =service.users().messages().get(userId = 'me',id=message['id']).execute()
            print(msg['snippet'])
            str=msg['snippet']
            speak(str)
            print('\n')
            time.sleep(2)
        speak("do you want to perform another action")
        if take_information()=="yes" :
            choice()
        else:
            speak('thank you have a nice day')




def speak(text):

    engine_start.say(text)
    engine_start.runAndWait()

def password():
    passwords=['hi alexa','hello alexa','hey assistant']
    speak("Tell me the passwords before start")
    key=take_information()
    try:
        for phrase in passwords:
            if phrase in key:
                return True
        return False
    except:
        speak("not able to recognize the password")

def wishMe():
    hour = int(datetime.datetime.now().hour)
    if hour >= 0 and hour < 12:
        speak("Good Morning!")


    elif hour >= 12 and hour < 18:
        speak("Good Afternoon!")

    else:
        speak("Good Evening!")

    speak("I am alexa Sir I am your email assistant")



def take_information():
    try:

        with sr.Microphone() as source:
            speak("speak")
            print('listening........')
            listener.energy_threshold=10000
            listener.adjust_for_ambient_noise(source,1.2)
            voice = listener.listen(source)


            # Here the information given by the user coverting audio to text by the help of google API
            information = listener.recognize_google(voice)
            print(information)
            return information.lower()

    except:
        pass


def sending_mail(receiver, subject_of_mail, message):
    #creating server (server name,port number)
    server = smtplib.SMTP('smtp.gmail.com', 587)
    #we are saying to the server thar you can trust me starttls===> transport layer security
    server.starttls()
    # Make sure to give app access in your Gmail account
    #logging to the gmail account
    server.login('vickysingh31561@gmail.com','*********')
    email = EmailMessage()
    email['From'] = 'vickysingh31561@gmail.com'
    email['To'] = receiver
    email['Subject'] = subject_of_mail
    #set the content of email i.e.body
    email.set_content(message)
    #server will send the email
    server.send_message(email)

#we use dictionary so that when we will say name it just takes email id on the basis of key value pair
email_dictionary = {
    'david': 'www.vickysingh31561@gmail.com',
    'akon': 'akon234@gmail.com',
    'ajay': 'ajay456@gmail.com',
    'jyoti': 'jyoti231@gmail1.com',
    'irene': 'irene@redvelvet.com'
    # and many more
}

#it will talk to us
def getting_mail_information():
    speak('Whom you want to send email?')

    #it will take the name and searches the name in email list and return value i.e, email id
    try:
        key = take_information()
        value = email_dictionary[key]
    except:
        speak("sorry sir name is not found")
        return 0
    print(value)
    speak('Tell me the subject of your email?')

    subject_of_mail = take_information()
    speak('Tell me the text in your email')
    message = take_information()
    sending_mail(value, subject_of_mail, message)
    playsound("Oppo Message Notification.mp3")
    speak("do you want to perform another action")
    if take_information() == "yes":
        choice()
    else:
        speak('thank you have a nice day')



wishMe()
if password()==True:
    speak("password matched")
    main()
else:
    speak("password does not matched")