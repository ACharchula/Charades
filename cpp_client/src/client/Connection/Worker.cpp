#include <QTimer>
#include <QThread>

#include <QDebug>
#include <fstream>
#include "Worker.h"
#include "client.h"
#include "../Consts.h"

const int lineFeed = 10;
const size_t MAXMESSAGESIZE = 1000 * 1000 - 20;
extern const std::string TEXT;

Worker::Worker(Client* client, QObject* parent) : client(client), QObject(parent), gameState(Other) {}

void Worker::writer() {
    int nextChar;
    forever {
        if(!errorStatus){
            std::string message;
            do {
                nextChar = getchar();
                message += static_cast<char>(nextChar);
            } while (nextChar != lineFeed && message.size() != MAXMESSAGESIZE);

            if (message.size() == MAXMESSAGESIZE)
                message += '\0';
            if (message[message.size() - 1] == lineFeed)
                message = message.substr(0, message.size() - 1);

            try{
                client->send(message, TEXT);
            }catch(const std::runtime_error& error){
                errorStatus = true;
                throwException(QString::fromStdString(error.what()));
                continue;
            }
        }
    }
}

void Worker::reader() {
    std::pair<std::unique_ptr<Message>, std::unique_ptr<Message>> data;
    forever {
        if(!errorStatus){
            try{
                data = client->receive();
            } catch(const std::runtime_error& error){
                errorStatus = true;
                throwException(QString::fromStdString(error.what()));
                continue;
            }

            if (data.first->equal(WELCOME)){
                emit statement(QString::fromStdString(WELCOME));
            } else if (data.first->equal(TABLELIST)){
                if(data.second != nullptr)
                    emit tableList(QString::fromStdString(data.second->getValue()));
                else
                    emit tableList(QString::fromStdString(""));
            } else if (data.first->equal(CHAT)){
                emit receiveMessage(QString::fromStdString(data.second->getTextMessage()));
            } else if (data.first->equal(WAIT)){
                emit statement(QString::fromStdString(WAIT));
            } else if (data.first->equal(END)){
                emit solution(QString::fromStdString(data.second->getClue()));
            } else if (data.first->equal(READY)){
                emit ready(QString::fromStdString(data.second->getValue()));
            } else if (data.first->equal(DRAW)){
                emit draw(QString::fromStdString(data.second->getValue()));
            } else if (data.first->equal(PING)){
                emit statement(QString::fromStdString(PING));
            } else if (data.first->equal(TABLECREATED)){
                emit tableCreated(QString::fromStdString(data.second->getValue()));
            } else if (data.first->equal(FAIL)){
                emit statement(QString::fromStdString(FAIL));
            } else if (data.first->equal(ABORT)){
                emit solution(QString::fromStdString(data.second->getAborted()));
            } else if (data.first->equal(STATS)){
                emit stats(QString::fromStdString(data.second->getValue()));
            } else if (data.first->equal(SET) || data.first->equal(UPDATE)){
                emit updateScene(QByteArray(data.second->getValue().data(), int(data.second->getValue().size())));
            }   else{
                qDebug() << "Ops.. Something was going wrong. Update your client app.";
            }
        }
    }
}

void Worker::sendFrame(QByteArray byteArray){
    if(!errorStatus){
        std::string data(byteArray.constData(), static_cast<unsigned long>(byteArray.length()));
        client->send(data, SET);
    }
}

void Worker::sendTextMessage(QString message) {
    if(!errorStatus){
        std::string data = message.toStdString();
        client->send(data, TEXT);
    }
}

void Worker::sendRequest(QString request) {
    if(!errorStatus){
        std::string req = request.toStdString();
        client->send("", req);
    }
}

void Worker::enterTable(QString table) {
    if(!errorStatus){
        std::string req = table.toStdString();
        client->send(req, ENTER);
    }
}

void Worker::changeErrorStatus(QString newStatus) {
    errorStatus = newStatus.toStdString() == "error";
}
