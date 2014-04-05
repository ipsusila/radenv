#ifndef IMODEL_H
#define IMODEL_H

#include <QString>
#include <QIODevice>
#include <QIcon>
#include <QGraphicsItem>
#include "kport.h"
#include "kmodelinfo.h"
#include "klocationport.h"
#include "kreportport.h"
#include "iuserinput.h"
#include "kcalculationinfo.h"

/**
 * @brief Base class for generic models
 */
class K_CORE_EXPORT IModel : public QGraphicsItem
{
private:
    int _tagId;
    KLocationPort * _locPort;
    KReportPort * _repPort;
    KModelInfo _info;
    int _sourceDistance;
    IModel * _visitor;
    IModelFactory * _factory;

    IModel(const IModel& model);
protected:
    virtual void mouseDoubleClickEvent (QGraphicsSceneMouseEvent * event);
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    virtual IUserInput * createUserInputWidget(QWidget * parent = 0);

    virtual bool needLocation() const;
    virtual bool needReport() const;
    virtual void defineParameters();
    virtual bool allocateIoPorts();
    virtual bool allocateLocationPorts();
    virtual bool allocateReportPorts();
    virtual void arrangePorts();
    virtual bool promptUserInputs();
    virtual void userInputsFinished(bool accepted);
    void setInfo(const KModelInfo& i);
    void setLocationPort(KLocationPort * port);

    IModel(IModelFactory * fact, const KModelInfo& inf);
public:
    enum { Type = UserType + TYPE_MODEL};
    static SPadding Padding;    ///< Padding between bounding rect and this item

    virtual ~IModel();

    virtual bool verify(int * err = 0, int * warn = 0) = 0;
    virtual bool load(QIODevice * io) = 0;
    virtual bool save(QIODevice * io) = 0;
    virtual KDataArray result() const = 0;
    virtual KData modelData(const Quantity & sym) const = 0;
    virtual bool calculate(const KCalculationInfo& ci) = 0;
    //virtual SymbolList outputSymbols() = 0;

    virtual void generateReport();
    virtual void refresh();
    virtual const KPortList & inputs() const;
    virtual const KPortList & outputs() const;
    virtual QString displayText() const;
    virtual bool isSource() const;
    virtual IModel * copyTo(KModelScene * mscene) const;

    int type() const;
    void askUserParameters();
    KData data(const Quantity & sym) const;
    int tagId() const;
    void setTagId(int id);
    QString tagName(const QString& post="") const;
    IModelFactory * factory() const;
    KModelInfo info() const;
    KLocationPort * locationPort() const;
    KReportPort * reportPort() const;
    KReport * report() const;
    bool initialize();
    KLocation location() const;
    void removeConnections();
    DataArrayList inputResults() const;

    //visit
    int sourceDistance() const;
    void visit(IModel * visitor);
    const IModel * latestVisitor() const;

    virtual KDataGroupArray * userInputs();
    virtual QRectF modelRect() const;
    virtual QRectF boundingRect () const;
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

    //check whether connected with
    template <class T>
    bool connectedWith() const
    {
        int sid = static_cast<T*>(0)->SerialId;
        const KPortList & inpPorts = inputs();
        for(int k = 0; k < inpPorts.size(); k++) {
            IModel * m = inpPorts.at(k)->model();
            if (m == 0)
                continue;

            if (sid == m->info().serialId())
                return true;
        }
        return false;
    }
};


#endif // IMODEL_H
