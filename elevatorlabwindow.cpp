#include "elevatorlabwindow.h"
#include "ui_elevatorlabwindow.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

ElevatorLabWindow::ElevatorLabWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ElevatorLabWindow)
{
    ui->setupUi(this);

    //Set up constants
    //Positive means up for everything

    /// Mass of the mass, kilograms
    const double mass = 0.5;

    /// Floor height, meters
    const double floorHeight = 4.1;

    /// Initial height above the ground, meters
    //TODO when the algorithm is working: Change this to the
    const double initialHeight = 0;

    /// Time elapsed between lines in the CSV file, seconds
    const double lineTime = 0.1;

    //Set up loop variables

    /// Acceleration, meters/second^2
    double acceleration = 0;

    /// Velocity, meters/second
    double velocity = 0;

    /// Height, meters
    double height = initialHeight;

    QFile csvFile("/Users/samcrow/Downloads/elevator_lab.csv");
    csvFile.open(QIODevice::Text | QIODevice::ReadOnly);

    //Read and ignore the first line (headers)
    csvFile.readLine();

    //Open the output file in the same directory as the input file
    QFileInfo csvInfo(csvFile);
    QString outPath = csvInfo.absoluteDir().path() + "/elevator_out.csv";
    qDebug() << outPath;
    QFile outFile(outPath);
    outFile.open(QIODevice::Text | QIODevice::Truncate | QIODevice::WriteOnly);
    outFile.write("Time, Acceleration, Velocity, Corrected Velocity, Height\n");

    //Iterate through every line
    while(true) {

        QString line = csvFile.readLine();
        //Exit condition: Leave the loop if the end of file is reached
        if(line.isEmpty()) {
            break;
        }

        QStringList parts = line.split(',');

        double time = parts.at(0).trimmed().toDouble();

        //The downward force, in newtons, excluding the force of gravity, that the 0.5 kg mass applies to the elevator
        //If this force is negative, the elevator is accelerating upwards and applying an equal and opposite force to the mass
        double forceOnElevator = parts.at(2).trimmed().toDouble();

        //The equal and opposite force that the elevator applies to the mass to accelerate it
        double forceOnMass = -forceOnElevator;

        //f = m a
        //a = f / m
        double acceleration = forceOnMass / mass;

        velocity += acceleration * lineTime;

        /// The velocity, corrected to reduce accumulated error
        double correctedVelocity = velocity - getError(time);

        height += correctedVelocity * lineTime;


        qDebug() << "Time" << time << "\tVelocity" << velocity << "\tCorrected velocity" << correctedVelocity << "\tError" << getError(time) << "\tHeight" << height;

        //Write data to the CSV file
        outFile.write(QByteArray::number(time));
        outFile.write(",");
        outFile.write(QByteArray::number(acceleration));
        outFile.write(",");
        outFile.write(QByteArray::number(velocity));
        outFile.write(",");
        outFile.write(QByteArray::number(correctedVelocity));
        outFile.write(",");
        outFile.write(QByteArray::number(height));
        outFile.write("\n");

    }


    outFile.close();
    csvFile.close();
}


double ElevatorLabWindow::getError(double time) {
    //ax^2+bx+c
    const double a = -8.0797E-5;
    const double b = -0.0391;
    const double c = 0.0368;

    double error = a * (time * time) + b * time + c;
    return error;
}

ElevatorLabWindow::~ElevatorLabWindow()
{
    delete ui;
}
