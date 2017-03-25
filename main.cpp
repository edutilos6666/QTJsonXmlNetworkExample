#include "mainwindow.h"
#include <QApplication>
#include <QtXml>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QString>
#include <QList>
#include <memory>
#include <QStringList>
#include <QXmlQuery>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFileInfo>
#include <QMainWindow>
#include <QWidget>
#include <QPlainTextEdit>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

using namespace std;

void testXml() {
  QFile file("person.xml");
  file.open(QIODevice::ReadWrite);

  QTextStream stream(&file);
  stream << "hello world" << endl
         << "bye world" << endl;

  file.close();

  file.open(QIODevice::ReadOnly);
  QTextStream stream2(&file);
  while(!stream2.atEnd()){
      qDebug() << stream2.readLine();
  }



}


class Person {
public:
    Person() {

    }

    Person(long id, QString name, int age, double wage) :
        id(id), name(name), age(age), wage(wage) {

    }

    ~Person()  {

    }

public:
 long id;
 QString name;
 int age;
 double wage;
};


void testXml2() {
   QString fileName = "person.xml";
   QFile file(fileName);
   file.open(QIODevice::WriteOnly| QIODevice::Text);
   QTextStream out(&file);
   out << "<People>" << endl;

    QList<shared_ptr<Person>> personList ;
    personList.push_back(shared_ptr<Person>(new Person(1, "foo", 10, 100.0)));
    personList.push_back(shared_ptr<Person>(new Person(2, "bar", 20 , 200.0)));
    personList.push_back(shared_ptr<Person>(new Person(3, "bim", 30 , 300.0)));
    for(shared_ptr<Person> p: personList) {
         out << "<Person>" << endl;
         long id = p->id ;
         QString name = p->name ;
         int age = p->age;
         double wage = p->wage;
         out << "<Id>" << id << "</Id>" << endl;
         out << "<Name>" << name << "</Name>" << endl;
         out << "<Age>" << age << "</Age>" << endl;
         out << "<Wage>" << wage << "</Wage>" << endl;
         out << "</Person>" << endl;
    }

    out << "</People>" << endl;
   file.close();

   file.open(QIODevice::ReadOnly  | QIODevice::Text);
   QTextStream in(&file);
   while(!in.atEnd())  {
       qDebug() << in.readLine();
   }
   file.close();
}

void testXml3() {
   QString fileName = "person.xml";
   QDomDocument doc ;
   QFile file(fileName);
   if(!file.open(QIODevice::ReadOnly | QIODevice::Text) || !doc.setContent(&file)) return ;

   QDomNodeList list = doc.elementsByTagName("Person");
   for(int i=0; i < list.size(); ++i) {
       QDomNode node = list.item(i);
       QDomElement elemId = node.firstChildElement("Id");
       QDomElement elemName = node.firstChildElement("Name");
       QDomElement elemAge = node.firstChildElement("Age");
       QDomElement elemWage = node.firstChildElement("Wage");
       if(elemId.isNull() || elemName.isNull() || elemAge.isNull() || elemWage.isNull()) continue;
       long id = elemId.text().toLong();
       QString name = elemName.text();
       int age = elemAge.text().toInt();
       double wage = elemWage.text().toDouble();

       qDebug() << id << ", " << name << ", " << age << ", " << wage ;
   }
}

 void printTokenTypeName(QXmlStreamReader::TokenType tokenType)  {
   QString res = "" ;
     switch(tokenType) {
     case QXmlStreamReader::NoToken:
         res = "NoToken";
         break;
     case QXmlStreamReader::Invalid:
         res = "Invalid";
         break;
     case QXmlStreamReader::StartDocument:
         res = "StardtDocument";
         break;
     case QXmlStreamReader::EndDocument:
         res = "EndDocument";
         break;
     case QXmlStreamReader::StartElement:
         res = "StartElement";
         break;
     case QXmlStreamReader::EndElement:
         res = "EndElement";
         break;
     case QXmlStreamReader::Characters:
         res = "Characters";
         break;
     case QXmlStreamReader::Comment:
         res = "Comment";
         break;
     case QXmlStreamReader::DTD:
         res = "DTD";
         break;
     case QXmlStreamReader::EntityReference:
         res = "EntityReference";
         break;
     case QXmlStreamReader::ProcessingInstruction:
         res = "ProcessingInstruction";
         break;
     }

     qDebug() << res;
 }

void testXml4() {
   QString fileName = "person.xml";
   QFile file(fileName);
   if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) return ;
   QXmlStreamReader reader ;
   reader.setDevice(&file);

   /*
    *   NoToken = 0,
       Invalid,
       StartDocument,
       EndDocument,
       StartElement,
       EndElement,
       Characters,
       Comment,
       DTD,
       EntityReference,
       ProcessingInstruction
       */

    while(!reader.atEnd())  {
      QXmlStreamReader::TokenType  tokenType =  reader.readNext();

      printTokenTypeName(tokenType) ;
    }





     file.close();
     file.open(QIODevice::ReadOnly | QIODevice::Text);
reader.setDevice(&file);

  if(reader.readNextStartElement() && reader.name() == "People") {
//      while(reader.readNextStartElement() && reader.name() != "Person") {
//        qDebug() << "not person" ;
//      }
      while(!reader.atEnd()) {
      if(reader.readNextStartElement() && reader.name() == "Person") {
          long id;
          QString name;
          int age ;
          double wage;
          while(reader.readNextStartElement()) {
              if(reader.name() == "Id") {
//                  reader.readNext();
//                  id = reader.text().toLong();
                 id =  reader.readElementText().toLong();
              } else if(reader.name() == "Name") {
//                  reader.readNext();
//                  name = reader.text().toString();
                  name = reader.readElementText();
              } else if(reader.name() == "Age") {
//                  reader.readNext();
//                  age = reader.text().toInt();
                  age = reader.readElementText().toInt();
              } else if(reader.name() == "Wage") {
//                  reader.readNext();
//                  wage = reader.text().toDouble();
                  wage = reader.readElementText().toDouble();
              }

          }
           qDebug() << id << ", " << name << ", " << age << ", " << wage ;
      } else {
          reader.readNextStartElement();
      }
      }
  }

}

void testXml5() {
    QString fileName= "person.xml";
  //  QString fileName = "C:/Users/edutilos/Desktop/person.xml";
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QFileInfo *info = new QFileInfo(file);
    QString filePath = info->absoluteFilePath();
    file.reset();
   // file.close();

   filePath = fileName;
    qDebug()<< filePath;
  QString pattern = "doc('%1')//People//%2/string()";
    //QString pattern = "doc('person.xml')//%1/string()";
    QStringList idList, nameList , ageList , wageList ;
  //QtMessageHandler handler ;
  QXmlQuery query;

 // query.setFocus(QUrl("person.xml"));
  query.setQuery(pattern.arg(filePath).arg("Id"));
//  query.setMessageHandler(&handler);
  bool res = query.evaluateTo(&idList);



  query.setQuery(pattern.arg(filePath).arg("Name"));
  query.evaluateTo(&nameList);

  query.setQuery(pattern.arg(filePath).arg("Age"));
  query.evaluateTo(&ageList);

  query.setQuery(pattern.arg(filePath).arg("Wage"));
  query.evaluateTo(&wageList);
  qDebug() << idList;
  qDebug() << nameList;
  qDebug() << ageList;
  qDebug() << wageList;
}

void testXml6()  {
    QString pattern = QString("doc('%1')//People/Person/string-join((Id, Name, Age, Wage)/string(), ',')")
   //QString pattern = QString("doc('%1')//People/Person/string-join((Id/string(), Name/string(), Age/string(), Wage/string()), ',')")
           .arg("person.xml");
   QStringList list ;
   QXmlQuery query;
   query.setQuery(pattern);
   query.evaluateTo(&list);
   //qDebug() << list;

   for(QString& str: list) {
       QStringList props = str.split(",");
       long id = props[0].toLong();
       QString name = props[1];
       int age = props[2].toInt();
       double wage = props[3].toDouble();

       qDebug() << id << "; " << name << "; " << age << "; " << wage;
   }
}



/*
 *   HttpStatusCodeAttribute,
        HttpReasonPhraseAttribute,
        RedirectionTargetAttribute,
        ConnectionEncryptedAttribute,
        CacheLoadControlAttribute,
        CacheSaveControlAttribute,
        SourceIsFromCacheAttribute,
        DoNotBufferUploadDataAttribute,
        HttpPipeliningAllowedAttribute,
        HttpPipeliningWasUsedAttribute,
        CustomVerbAttribute,
        CookieLoadControlAttribute,
        AuthenticationReuseAttribute,
        CookieSaveControlAttribute,
        MaximumDownloadBufferSizeAttribute, // internal
        DownloadBufferAttribute, // internal
        SynchronousRequestAttribute, // internal
        BackgroundRequestAttribute,
        SpdyAllowedAttribute,
        SpdyWasUsedAttribute,
        EmitAllUploadProgressSignalsAttribute,
        FollowRedirectsAttribute,
        HTTP2AllowedAttribute,
        HTTP2WasUsedAttribute,

        User = 1000,
        UserMax = 32767
 * */


void readXmlFile(QFile& file);
void readXmlFile2(QFile& file);
void handleReply(QNetworkReply* reply, QNetworkAccessManager* manager) {
    qDebug() << "inside handle Reply" ;
   // QNetworkReply* reply =  manager->get(req);
    auto attr = reply->attribute(QNetworkRequest::Attribute::HttpStatusCodeAttribute);
    qDebug() << attr.toString();
    QString line ;
//    while(!reply->atEnd()) {
//    line = reply->readLine();
//qDebug() << line ;
//    }
  QFile file("onlineXml1.xml");
  file.open(QIODevice::WriteOnly);
  file.write(reply->readAll());
  file.close();


 file.setFileName("onlineXml2.xml");
  file.open(QIODevice::ReadOnly| QIODevice::Text);
    readXmlFile2(file);

    file.close();

}

/*
 *    <AdSystem>Acudeo Compatible</AdSystem>
    <AdTitle>NonLinear Test Campaign 1</AdTitle>
    <Description>NonLinear Test Campaign 1</Description>
    <Survey>http://mySurveyURL/survey</Survey>
    <Error>http://myErrorURL/error</Error>
    <Impression>http://myTrackingURL/impression</Impression>
 *
 * */


/*
 * 	<Creative AdID="602678-NonLinear">
            <NonLinearAds>
                <TrackingEvents>
                    <Tracking event="creativeView">http://myTrackingURL/nonlinear/creativeView</Tracking>
                    <Tracking event="expand">http://myTrackingURL/nonlinear/expand</Tracking>
                    <Tracking event="collapse">http://myTrackingURL/nonlinear/collapse</Tracking>
                    <Tracking event="acceptInvitation">http://myTrackingURL/nonlinear/acceptInvitation</Tracking>
                    <Tracking event="close">http://myTrackingURL/nonlinear/close</Tracking>
                </TrackingEvents>
                <NonLinear height="50" width="300" minSuggestedDuration="00:00:15">
                    <StaticResource creativeType="image/jpeg">
                    http://demo.tremormedia.com/proddev/vast/50x300_static.jpg
                    </StaticResource>
                    <NonLinearClickThrough>http://www.tremormedia.com</NonLinearClickThrough>
                </NonLinear>
                <NonLinear height="50" width="450" minSuggestedDuration="00:00:20">
                    <StaticResource creativeType="image/jpeg">
                    http://demo.tremormedia.com/proddev/vast/50x450_static.jpg
                    </StaticResource>
                    <NonLinearClickThrough>http://www.tremormedia.com</NonLinearClickThrough>
                </NonLinear>
            </NonLinearAds>
        </Creative>
 * */

QString parseNonLinear(QDomNode nonLinear) {
    QString res ;
    QTextStream out(&res);
   QDomNamedNodeMap mapAttrs =   nonLinear.attributes();
    QString height = mapAttrs.namedItem("height").toAttr().value(),
            width = mapAttrs.namedItem("width").toAttr().value(),
            minSuggestedDuration = mapAttrs.namedItem("minSuggestedDuration").toAttr().value();

    QDomElement staticResource = nonLinear.firstChildElement("StaticResource");
     QString creativeType = staticResource.attributes().namedItem("creativeType").toAttr().value();
     QString textStaticResource = staticResource.text();
     QString textNonLinearClickThrough = nonLinear.firstChildElement("NonLinearClickThrough").text();

     out << "NonLinear [" << height << ", " << width << ", " << minSuggestedDuration << "]" << endl
         << "-> StaticResource [" << creativeType << "] = " << textStaticResource << endl
         << "-> NonLinearClickThrough []" << textNonLinearClickThrough << endl;
     return res ;
}

QString parseNonLinearAds(QDomNode nonLinearAds) {
    QString res ;
    QTextStream out(&res);
    out << "TrackingEvents: \n" ;
    QDomNodeList events = nonLinearAds.firstChildElement("TrackingEvents").childNodes();
    for(int i=0; i< events.count(); ++i) {
        QDomNode event = events.at(i);
        QString value = event.attributes().namedItem("event").toAttr().value();
        QString textEvent = event.toElement().text();
       out << "[" <<  value << ", " << textEvent << "]" << endl ;
    }
    //out << "] ";
    out << endl;
      QDomNodeList list = nonLinearAds.childNodes();

      out  << parseNonLinear(list.at(1))
          << parseNonLinear(list.at(2));

    return res ;
}

QString parseCreatives(QDomNodeList list) {
QString res = "";
QTextStream out(&res);

for(int i=0; i< list.count(); ++i) {
    QDomNode nodeCreative = list.at(i);
    QString adID = nodeCreative.attributes().namedItem("AdID").toAttr().value();
    QDomNode nonLinearAds = nodeCreative.firstChildElement("NonLinearAds");
     out << "Creative [" << adID << "]" << endl
         << parseNonLinearAds(nonLinearAds) << endl ;
}

return res ;
}

void readXmlFile2(QFile& file) {
QDomDocument doc ;
doc.setContent(&file);
//doc.elementsByTagName("InLine");
QDomNode nodeInLine = doc.elementsByTagName("InLine").at(0);
QDomElement elAdSystem = nodeInLine.firstChildElement("AdSystem");
QDomElement elAdTitle = nodeInLine.firstChildElement("AdTitle");
QDomElement elDescription = nodeInLine.firstChildElement("Description");
QDomElement elSurvey = nodeInLine.firstChildElement("Survey");
QDomElement elError = nodeInLine.firstChildElement("Error");
QDomElement elImpression = nodeInLine.firstChildElement("Impression");
 qDebug() << "AdSystem = " << elAdSystem.text();
 qDebug() << "AdTitle = " << elAdTitle.text();
 qDebug() << "Description = " << elDescription.text()  << endl
          << "Survey = " << elSurvey.text() << endl
          << "Impression = " << elImpression.text() << endl
          << "Error = " << elError.text() ;


QDomElement elCreatives =  nodeInLine.firstChildElement("Creatives");
QDomNodeList creatives = elCreatives.childNodes();
   qDebug() << "creatives count = " << creatives.count() ;


   QDomNode nodeVAST =  doc.elementsByTagName("VAST").at(0);
   QDomNamedNodeMap attributesVAST =  nodeVAST.attributes();
  QDomAttr attrVAST = attributesVAST.namedItem("version").toAttr();
   qDebug() << attrVAST.value() ;

   qDebug() << parseCreatives(creatives);
}




void readXmlFile(QFile& file) {
    //QString pattern = "doc('%1')//%2/string()";
    QString pattern = "doc('%1')/VAST/Ad/InLine/%2/string()";
    QFileInfo* info = new QFileInfo();
    info->setFile(file);
    //pattern = pattern.arg(info->absoluteFilePath());
    QString filePath = info->absoluteFilePath();
     qDebug() << "filePath = " << filePath ;
     filePath = file.fileName();
     qDebug() << "filePath = " << filePath;
    QXmlQuery query;
     QString AdSystem, AdTitle, Description , Survey, Error, Impression;

     query.setQuery(pattern.arg(filePath).arg("AdSystem"));
     query.evaluateTo(&AdSystem);
     query.setQuery(pattern.arg(filePath).arg("AdTitle"));
     query.evaluateTo(&AdTitle);

     qDebug() << "AdSystem = " << AdSystem;
     qDebug() << "AdTitle = " << AdTitle ;
}


void testNetwork() {
    QNetworkAccessManager* manager = new QNetworkAccessManager();
      QNetworkRequest  req ;
 //= new QNetworkRequest() ;
 // QString urlStr = "http://doc.qt.io/qt-5/qfile.html";
  QString urlStr = "http://demo.tremorvideo.com/proddev/vast/vast_inline_nonlinear.xml";
      req.setSslConfiguration(QSslConfiguration::defaultConfiguration());
  req.setUrl(QUrl(urlStr));

   //QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), 0, SLOT(handleReply(QNetworkReply*)));
  QNetworkReply* reply =  manager->get(req);
  QObject::connect(manager, &QNetworkAccessManager::finished, [&reply, &manager]() {
      handleReply(reply, manager);
  });
}



void handleReply(QNetworkAccessManager* manager , QNetworkReply* reply) {
    qDebug() << "inside handleReply" ;
   QTemporaryFile file;
   // file.open();
    //qDebug() << reply->readAll();
  // file.write(reply->readAll());
  // file.flush();
  // file.seek(0);
   QString line ;
   QString str;

   file.open();
   file.write(reply->readAll());

   file.flush();
   file.seek(0);
   while(!file.atEnd()) {
       qDebug() << file.readLine();
   }

   file.close();

//   QFile file2("test.out");
//   file2.open(QIODevice::WriteOnly | QIODevice::Text);
// file2.write(reply->readAll());
//   file2.close();

//   file2.open(QIODevice::ReadOnly | QIODevice::Text);
//     while(!file2.atEnd()) {
//         qDebug() << file2.readLine();
//     }
//   file2.close();


//     while(!reply->atEnd()) {
//         qDebug() << reply->readLine() ;
//     }

//   QTextStream stream(&str);
//   while(!file.atEnd()) {
//       line = file.readLine();
//       stream << line << endl ;
//   }

//   qDebug() << str;
  // qDebug() << file.readAll() ;
}

void testNetwork2() {
  QNetworkAccessManager* manager = new QNetworkAccessManager();
  QNetworkRequest request ; //= new QNetworkRequest();
  request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
  QString urlStr = "https://forum.qt.io/topic/10972/solved-xml-readproblems/3";
  //QString urlStr = "http://doc.qt.io/qt-5/qfile.html";
  QUrl url(urlStr);
    request.setUrl(url);

  QNetworkReply *reply = manager->get(request);
  QObject::connect(manager, &QNetworkAccessManager::finished , [&manager, &reply](){
    handleReply(manager, reply);
  });
}


class Worker {
public:
    Worker(long id, QString name, int age, double wage);
    Worker();
    ~Worker();
    QString toString() const ;
public:
    long id;
    QString name;
    int age;
    double wage;
};


Worker::Worker(long id, QString name, int age, double wage):
id(id), name(name), age(age), wage(wage){

}

Worker::Worker() {

}

Worker::~Worker() {

}

QString Worker::toString() const  {
    QString res ;
    QTextStream out(&res);
    out << id << ", " << name << ", " << age << ", " << wage;
    return res ;
}

QList<Worker*> generateWorkers() {
    Worker *w1 = new Worker(1, "foo", 10 , 100.0),
           *w2 = new Worker(2, "bar", 20 , 200.0),
            *w3 = new Worker(3, "bim", 30, 300.0);
    QList<Worker*> list ;
    list.push_back(w1);
    list.push_back(w2);
    list.push_back(w3);
    list.append(new Worker(4, "edu", 40 , 400.0));
    list.append(new Worker(5, "tilos", 50, 500.0));
    list.append(new Worker(6, "pako", 60, 600.0));
    return list;
}


void createWorkerXml(QString& fileName, QList<Worker*> list) {
 QFile fileWorker(fileName);
 fileWorker.open(QIODevice::ReadWrite| QIODevice::Text);
 QTextStream out(&fileWorker);
 out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl; ;
 out << "<Workers>" << endl;

 for(Worker* w : list ) {
     long id = w->id ;
     QString name = w->name ;
     int age = w->age ;
     double wage = w->wage;
    out << "<Worker id = \"" << id << "\">" << endl;
    out << "<Name>" << name << "</Name>" << endl
        << "<Age>" << age << "</Age>" << endl
        << "<Wage>" << wage << "</Wage>" << endl
        << "</Worker>" << endl;
 }
 out << "</Workers>" << endl;

 for(Worker* w : list) {
     delete w ;
 }

 fileWorker.close();

}

void createWorkerXml() {
 QList<Worker*> list = generateWorkers();
 QString fileName = "workers.xml";
 createWorkerXml(fileName, list);
}


QList<Worker*> parseWorkerXml(QString fileName) {
  QList<Worker*> res ;
  QFile file(fileName);
  file.open(QIODevice::ReadWrite);
  QDomDocument doc ;
  doc.setContent(&file);

  QDomNodeList listWorkers = doc.firstChildElement("Workers").childNodes();
  qDebug() << "listWorkers Count = "<< listWorkers.count() ;
  for(int i=0; i< listWorkers.count(); ++i)  {
      qDebug() << "here" ;
      Worker* w = new Worker();
      QDomNode nodeWorker = listWorkers.at(i);
      long id = nodeWorker.attributes().namedItem("id").toAttr().value().toLong();
      QString name = nodeWorker.firstChildElement("Name").text();
      int age = nodeWorker.firstChildElement("Age").text().toInt();
      double wage = nodeWorker.firstChildElement("Wage").text().toDouble();
      w->id = id ;
      w->name = name;
      w->age = age ;
      w->wage = wage ;
      res.append(w);
  }

  file.close();
  return res ;
}



void testWorkerXml() {
    createWorkerXml();
    QString fileName = "workers.xml";
    QList<Worker*> list = parseWorkerXml(fileName);

    qDebug() << "*** all workers ***" ;
    for(Worker* w: list) {
        qDebug() << w->toString();
    }
}



//properties
QMainWindow *mainWindow ;
QWidget *centralWidget;
QLabel *lblTitle;
QPlainTextEdit *editDisplay;
QPushButton *btnCreate , *btnSave, *btnLoad;
QPushButton *btnSaveAsJson, *btnSaveAsJsonBin, *btnLoadFromJson, *btnLoadFromJsonBin;
QVBoxLayout *layout ;
QList<Worker*> workerList;
void addWidgets();
void registerEvents(QList<Worker*> workerList, QMainWindow *mainWindow);
void loadWorkers();
void fillWorkerList();
void saveWorkersAsJson(QString& fileName, QList<Worker*> list , bool isBinary) ;
QList<Worker*> loadFromJson(QString& fileName, bool isBinary) ;

void createMainWindow() {
   mainWindow = new QMainWindow();
   mainWindow->setWindowTitle("Test");
   mainWindow->resize(500, 500);
   centralWidget = new QWidget();
   mainWindow->setCentralWidget(centralWidget);

   layout = new QVBoxLayout();
   centralWidget->setLayout(layout);

  fillWorkerList();
  addWidgets();
  loadWorkers();
  registerEvents(workerList, mainWindow);
   mainWindow->show();
}


void addWidgets() {
  lblTitle = new QLabel("All Workers");
  layout->addWidget(lblTitle);
  editDisplay = new QPlainTextEdit();
  layout->addWidget(editDisplay);
  //buttons
   btnCreate = new QPushButton("Create new Worker...");
   layout->addWidget(btnCreate);
   btnSave = new QPushButton("Save...");
   layout->addWidget(btnSave);
   btnLoad = new QPushButton("Load...");
   layout->addWidget(btnLoad);

   btnSaveAsJson = new QPushButton("Save As Json...");
   btnSaveAsJsonBin = new QPushButton("Save As Json Bin...");
   btnLoadFromJson = new QPushButton("Load From Json...");
   btnLoadFromJsonBin = new QPushButton("Load From Json Bin...");
   layout->addWidget(btnSaveAsJson);
   layout->addWidget(btnSaveAsJsonBin);
   layout->addWidget(btnLoadFromJson);
   layout->addWidget(btnLoadFromJsonBin);
}


void insertWorker(long id, QString name ,int age, double wage) {
    Worker* w = new Worker(id, name, age, wage);
    workerList.push_back(w);
    loadWorkers();
}

void handleLoad() {
    QString dir = "C:\QtNew\myProjects\build-Test-Desktop_Qt_5_8_0_MSVC2015_64bit-Debug";
   QString filter ="XML Files (*.xml)";
   QString fileName = QFileDialog::getOpenFileName(mainWindow, "Open Xml File", dir, filter) ;
   qDebug() << fileName ;
workerList = parseWorkerXml(fileName);
    loadWorkers();
}


void handleSave() {
QString dir = "C:\QtNew\myProjects\build-Test-Desktop_Qt_5_8_0_MSVC2015_64bit-Debug";
QString filter = "XML Files(*.xml)";
QString fileName = QFileDialog::getSaveFileName(mainWindow , "Save XMl File", dir, filter);

QList<Worker*> workers ;
QString content = editDisplay->document()->toPlainText();
QStringList splitted = content.split("\n");
for(QString &line: splitted) {
    QStringList props = line.split(";");
    long id = props[0].toLong();
    QString name = props[1];
    int age = props[2].toInt();
    double wage = props[3].toDouble();
     workers.append(new Worker(id, name, age, wage));
}
//void createWorkerXml(QString& fileName, QList<Worker*> list);
   createWorkerXml(fileName, workers);
}


QList<Worker*> displayToWorkers(){
    QList<Worker*> list ;
    QString content = editDisplay->document()->toPlainText();
    QStringList splitted = content.split("\n");
    for(QString sp: splitted) {
        QStringList props = sp.split(";");
        long id = props[0].toLong();
        QString name = props[1];
        int age = props[2].toInt();
        double wage = props[3].toDouble();
        list.append(new Worker(id, name, age, wage));
    }

    return list;
}

void handleSaveAsJson()  {
    QString caption = "Save As Json";
    QString dir = "C:\QtNew\myProjects\build-Test-Desktop_Qt_5_8_0_MSVC2015_64bit-Debug";
    QString filter = "Json Files(*.json)";
   QString fileName = QFileDialog::getSaveFileName(mainWindow, caption , dir, filter);

 //    QList<Worker*> workers = displayToWorkers();

     saveWorkersAsJson(fileName, workerList, false);
}


void handleSaveAsJsonBin()  {
    QString caption = "Save As Json Bin";
    QString dir = "C:\QtNew\myProjects\build-Test-Desktop_Qt_5_8_0_MSVC2015_64bit-Debug";
    QString filter = "Json Files(*.json)";
   QString fileName = QFileDialog::getSaveFileName(mainWindow, caption , dir, filter);

    // QList<Worker*> workers = displayToWorkers();

     saveWorkersAsJson(fileName, workerList, true);
}


void handleLoadFromJson() {
    QString caption = "Open As Json";
    QString dir = "C:\QtNew\myProjects\build-Test-Desktop_Qt_5_8_0_MSVC2015_64bit-Debug";
    QString filter = "Json Files(*.json)";
   QString fileName = QFileDialog::getOpenFileName(mainWindow, caption , dir, filter);

   workerList = loadFromJson(fileName, false);
   loadWorkers();
}

void handleLoadFromJsonBin() {
    QString caption = "Open As Json Bin";
    QString dir = "C:\QtNew\myProjects\build-Test-Desktop_Qt_5_8_0_MSVC2015_64bit-Debug";
    QString filter = "Json Files(*.json)";
   QString fileName = QFileDialog::getOpenFileName(mainWindow, caption , dir, filter);

   workerList = loadFromJson(fileName, true);
   loadWorkers();
}


void registerEvents(QList<Worker*> workerList, QMainWindow* mainWindow) {
    QObject::connect(btnCreate, &QPushButton::clicked, [workerList](){
//        QList<Worker*> wList = workerList;
      QMainWindow *secondWindow = new QMainWindow();
      secondWindow->setWindowTitle("Create New Worker");
      secondWindow->resize(400 , 400);
      QWidget *widget = new QWidget();
      secondWindow->setCentralWidget(widget);

      QFormLayout *layout = new QFormLayout();
      widget->setLayout(layout);
      //id
      QLabel *lblId = new QLabel("Id: ");
      QLineEdit *editId = new QLineEdit();
      layout->addRow(lblId, editId);
      //name
      QLabel *lblName = new QLabel("Name: ");
      QLineEdit *editName = new QLineEdit();
      layout->addRow(lblName, editName);
      //age
      QLabel *lblAge =new QLabel("Age: ");
      QLineEdit *editAge = new QLineEdit();
      layout->addRow(lblAge, editAge);
      //wage
      QLabel *lblWage = new QLabel("Wage: ");
      QLineEdit *editWage = new QLineEdit();
      layout->addRow(lblWage, editWage);
      //buttons
      QPushButton *btnOk = new QPushButton("Ok");
      QPushButton *btnReset = new QPushButton("Reset");
      layout->addRow(btnOk, btnReset);

      //long id= editId->text().toLong();
      QObject::connect(btnOk , &QPushButton::clicked, [editId, editName, editAge, editWage, secondWindow](){
          long id = editId->text().toLong();
          QString name = editName->text();
          int age = editAge->text().toInt();
          double wage = editWage->text().toDouble();
          qDebug() << id << ", " << name << ", " << age << ", " << wage;
         insertWorker(id, name, age, wage);
         secondWindow->close();
      });


      QObject::connect(btnReset, &QPushButton::clicked, [editId, editName, editAge, editWage](){
          editId->clear();
          editName->clear();
          editAge->clear();
          editWage->clear();
      });

      secondWindow->show();

    });
    //end btnCreate

    /*
     *     static QString getOpenFileName(QWidget *parent = Q_NULLPTR,
                                   const QString &caption = QString(),
                                   const QString &dir = QString(),
                                   const QString &filter = QString(),
                                   QString *selectedFilter = Q_NULLPTR,
                                   Options options = Options());
     * */
    QObject::connect(btnLoad, &QPushButton::clicked, []() {
      handleLoad();
    });

    QObject::connect(btnSave, &QPushButton::clicked , [](){
        handleSave();
    });


    QObject::connect(btnSaveAsJson, &QPushButton::clicked, [](){
         handleSaveAsJson();
    });

    QObject::connect(btnSaveAsJsonBin, &QPushButton::clicked, [](){
        handleSaveAsJsonBin();
    });

    QObject::connect(btnLoadFromJson, &QPushButton::clicked, [](){
         handleLoadFromJson();
    });

    QObject::connect(btnLoadFromJsonBin, &QPushButton::clicked, [](){
        handleLoadFromJsonBin();
    });
}

void loadWorkers() {
    editDisplay->document()->clear();
  QString str;
  QTextStream out(&str);
  for(Worker* w: workerList) {
      long id = w->id;
      QString name = w->name;
      int age = w->age;
      double wage = w->wage;
      out << id <<";" << name <<";" << age <<";" << wage<< ";" << endl;
  }

  str = str.mid(0, str.count()-1);
  editDisplay->document()->setPlainText(str);
}


void fillWorkerList()  {
  QList<Worker*> list = generateWorkers();

  for(auto &w: list) {
      workerList.push_back(w);
  }
 }


void handleReply3(QNetworkReply* reply) {
   while(!reply->atEnd()) {
       qDebug() << reply->readLine();
   }
}

void testNetwork3() {
   QNetworkAccessManager *manager= new QNetworkAccessManager() ;
   QNetworkRequest *request = new QNetworkRequest();
   QNetworkReply *reply ;
   QString urlStr = "http://doc.qt.io/qt-5/qfiledialog.html";
   request->setSslConfiguration(QSslConfiguration::defaultConfiguration());
   request->setUrl(QUrl(urlStr));

   reply = manager->get(*request);
   QObject::connect(manager , &QNetworkAccessManager::finished, [reply]() {
     handleReply3(reply);
   });
}


void testJson1() {
  QJsonDocument doc;
  QList<Worker*> list = parseWorkerXml("workers.xml");
  QJsonValue jsonValue ("Workers");
  QJsonArray arr {jsonValue};


  for(Worker* w: list) {
      QJsonObject obj ;
      obj.insert("Id", w->id);
      obj.insert("Name", w->name);
      obj.insert("Age", w->age);
      obj.insert("Wage", w->wage);
      arr.append(obj);
  }


  QFile file("workersBinary.json");
  if(!file.open(QIODevice::ReadWrite)) return ;
   doc.setArray(arr);

  file.write(doc.toBinaryData());
  file.flush();
  file.seek(0);








  QJsonDocument doc2 = QJsonDocument::fromBinaryData(file.readAll());

  if(doc2.isArray()) {
      QJsonArray arr2 = doc2.array();
      for(auto it  = arr2.begin(); it != arr2.end(); ++it)  {
          QJsonValue value = *it ;
          if(value.isObject())  {
                QJsonObject obj = value.toObject();
                  QStringList keys = obj.keys();
                  for(QString key: keys) {
                      auto value = obj.value(key);
                    if(key == "Id" || key == "Age" || key == "Wage") {
                              qDebug() << key << value.toDouble() ;

                  } else if(key == "Name") {
                        qDebug() << key << value.toString();
                    }
                  }
          }
      }
  } else if(doc2.isObject()) {

  } else if(doc2.isEmpty()) {
      qDebug() << "doc2 is empty" ;
  } else if(doc2.isNull()) {
      qDebug() << "doc2 is null";
  }

  if(file.isOpen())
      file.close();


}




QList<Worker*> loadFromJson(QString& fileName, bool isBinary) {
  QList<Worker*> list ;
  QFile file(fileName);
  if(!file.open(QIODevice::ReadWrite))  return list ;



  QJsonDocument doc;
  if(isBinary) doc = QJsonDocument::fromBinaryData(file.readAll());
  else doc = QJsonDocument::fromJson(file.readAll());

  if(file.isOpen())
  file.close();

   if(doc.isArray()) {
       QJsonArray arrPeople = doc.array();
       for(QJsonValue value: arrPeople){
           if(value.isObject()) {
               QJsonObject person = value.toObject();
               long id = person.value("Id").toInt();
               QString name = person.value("Name").toString();
               int age = person.value("Age").toInt();
               double wage = person.value("Wage").toDouble();
               list.append(new Worker(id, name, age, wage));
           }
       }
   } else if(doc.isObject()) {
       qDebug() << "doc is object" ;
   } else if(doc.isEmpty()) {
       qDebug() << "doc is empty";
   } else if(doc.isNull()) {
       qDebug() << "doc is null" ;
   }


  return list;
}


void saveWorkersAsJson(QString& fileName, QList<Worker*> list , bool isBinary)  {
  QJsonDocument doc ;
  //parse list into json array
  QJsonArray arrPeople {QJsonValue("Workers")} ;
  for(Worker* w: list) {
      QJsonObject worker ;
      worker["Id"] = w->id ;
      worker["Name"] = w->name ;
      worker["Age"] = w->age;
      worker["Wage"] = w->wage;
      arrPeople.append(worker);
  }

  doc.setArray(arrPeople);

  QFile file(fileName);
  if(!file.open(QIODevice::ReadWrite)) {
      return ;
  }

  QByteArray resArr ;
  if(isBinary) resArr = doc.toBinaryData();
  else resArr = doc.toJson();
  file.write(resArr);

  if(file.isOpen())
      file.close();

}


void testJson2() {
  QString fileName = "workers.json";
  QString fileName2 = "workersBinary.json";

  QList<Worker*> workerList = loadFromJson(fileName, false);
  qDebug() << "*** all workers ***" ;
  for(Worker* w: workerList)
      qDebug() << w->toString();


  QList<Worker*> workerList2 = loadFromJson(fileName2, true);
  qDebug() << "*** all binary workers ***";
  for(Worker* w: workerList2)
      qDebug() << w->toString();



  fileName = "worker2.json";
  saveWorkersAsJson(fileName, workerList  , false);

  fileName2 = "worker2Binary.json";
  saveWorkersAsJson(fileName2, workerList, true);

  qDebug() << endl << endl;
  qDebug() << "after save";
  workerList = loadFromJson(fileName, false);
  qDebug() << "*** all workers ***" ;
  for(Worker* w: workerList)
      qDebug() << w->toString();

  workerList2 = loadFromJson(fileName2, true);
  qDebug() << "*** all binary workers ***";
  for(Worker* w: workerList2)
      qDebug() << w->toString();

}


int main(int argc, char *argv[])
{
   QApplication app(argc, argv);
//   qDebug() << QDir::tempPath() ;
//   // testNetwork2();
//   // testXml5();
//     testWorkerXml();
     createMainWindow();
//   testNetwork3();
   return app.exec();
//   testJson2();
}
