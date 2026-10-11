#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QTimer>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>
#include <nlohmann/json.hpp>
#include <fstream>
#include <qcoreapplication.h>
// #include <iostream>

using json = nlohmann::json;

class App: public QMainWindow {
private:
    int ppc = 1;
    double points = 0;
    double pps = 1;
public:
    json upgrades;
    App() {
        this->setWindowTitle("Generic as fuck clicker game");
        this->resize(960, 480);

        {
            std::ifstream upgradesFile("upgrades.json");
            this->upgrades = json::parse(upgradesFile);
            upgradesFile.close();
        }

        QScrollArea* shopPanel = new QScrollArea(this);
        shopPanel->setWidgetResizable(true);
        shopPanel->setStyleSheet(
            "QScrollArea {"
            "   border-radius: 10px;"
            "   border: 2px solid white;"
            "}"
        );
        QWidget* shopPanelWidget = new QWidget(shopPanel);
        shopPanelWidget->setStyleSheet("background-color: transparent");
        
        QVBoxLayout* shopPanelLayout = new QVBoxLayout(shopPanelWidget);
        shopPanelLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
        
        shopPanel->setWidget(shopPanelWidget);

        shopPanel->setGeometry(this->width() - 300, this->height() - 400, 300, 400);

        for (auto item: this->upgrades.items()) {
            std::string key = item.key();
            json value = item.value();
            double cost = value["cost"].get<double>();
            QPushButton* upgradeBtn = new QPushButton(QString::fromStdString("Name: " + key + "\nCost: " + std::to_string(cost)), shopPanel);
            upgradeBtn->setStyleSheet(
                "QPushButton {"
                "   background-color: rgb(75, 0, 255);"
                "   border: 2px solid white;"
                "   border-radius: 10px;"
                "}"
                "QPushButton:hover {"
                "   background-color: rgb(60, 0, 255);"
                "}"
                "QPushButton:pressed {"
                "   background-color: rgb(50, 0, 255);"
                "}"
            );

            if (value["type"] == "upgrade_ppc") {
                this->connect(upgradeBtn, &QPushButton::clicked, [this, value, cost]() {
                    if (this->points >= cost) {
                        this->ppc += value["ppc"].get<double>();
                        this->points -= value["cost"].get<double>();
                    }
                });
            }

            if (value["type"] == "upgrade_pps") {
                this->connect(upgradeBtn, &QPushButton::clicked, [this, value, cost]() {
                    if (this->points >= cost) {
                        this->pps += value["pps"].get<double>();
                        this->points -= value["cost"].get<double>();
                    }
                });
            }

            shopPanelLayout->addWidget(upgradeBtn);
        }

        QPushButton* clickMeBtn = new QPushButton("Click meeeee", this);
        clickMeBtn->setGeometry((int)this->width() / 2 - 100, (int)this->height() / 2 - 100, 100, 100);
        this->connect(clickMeBtn, &QPushButton::clicked, [this]() {
            this->points += this->ppc;
        });

        clickMeBtn->setStyleSheet(
            "QPushButton {"
            "   background-color: rgb(75, 0, 255);"
            "   border: 2px solid white;"
            "   border-radius: 10px;"
            "}"
            "QPushButton:hover {"
            "   background-color: rgb(60, 0, 255);"
            "}"
            "QPushButton:pressed {"
            "   background-color: rgb(50, 0, 255);"
            "}"
        );

        QLabel* pointsLbl = new QLabel("Points amount will be here", this);
        pointsLbl->setStyleSheet(
            "QLabel {"
            "   font-size: 20px;"
            "   font-weight: bold;"
            "}"
        );
        pointsLbl->move((int)this->width() / 2 - 200, 20);
        pointsLbl->setFixedWidth(400);

        QTimer *pointsPerSecondLoop = new QTimer(this);
        this->connect(pointsPerSecondLoop, &QTimer::timeout, [this, pointsLbl]() {
            this->points += this->pps / 62.5;
            pointsLbl->setText("Points: " + QString::number(this->points));
        });
        pointsPerSecondLoop->start(16);
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    App win;
    win.show();

    return QApplication::exec();
}