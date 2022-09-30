FROM python:3.10-slim

COPY . /docker-app

WORKDIR /docker-app

RUN pip3 install -r requirements.txt

ENV FLASK_APP=main.py
ENV FLASK_RUN_HOST=0.0.0.0
ENV FLASK_RUN_PORT=8080

ENV GOOGLE_CLOUD_PROJECT=bdcc-project1-346914

CMD [ "python3", "-m" , "flask", "run"]