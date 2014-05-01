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

class IModelPrivate;

/**
 * @brief Base class for generic models
 */
class K_CORE_EXPORT IModel : public QGraphicsItem
{
    friend class IModelPrivate;
public:
    enum { Type = UserType + TYPE_MODEL};
    static SPadding Padding;    ///< Padding between bounding rect and this item

    virtual ~IModel();

    virtual bool verify(int * err = 0, int * warn = 0) = 0;
    virtual bool load(QIODevice * io) = 0;
    virtual bool save(QIODevice * io) = 0;
    virtual KDataArray result() const = 0;
    virtual KData modelData(const Quantity & qty) const = 0;
    virtual bool calculate(const KCalculationInfo& ci) = 0;
    //virtual QuantityList outputQuantities() = 0;

    virtual void connectionModified(KPort * port, KConnector * con, bool connected);
    virtual void generateReport();
    virtual void refresh();
    virtual const KPortList & inputs() const;
    virtual const KPortList & outputs() const;
    virtual QString displayText() const;
    virtual bool isSource() const;
    virtual bool isSink() const;
    virtual IModel * copyTo(KModelScene * mscene) const;
    virtual void copyDataTo(IModel * model) const;

    int type() const;
    void askUserParameters();
    KData data(const Quantity & qty) const;
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
    ModelList connectedOutputModels() const;
    ConnectorList outputConnectors() const;

    //visit
    int sourceDistance() const;
    void visit(IModel * visitor);
    const IModel * latestVisitor() const;

    virtual KDataGroupArray * userInputs();
    virtual QRectF modelRect() const;
    virtual QRectF boundingRect () const;
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

protected:
    virtual void mouseDoubleClickEvent (QGraphicsSceneMouseEvent * event);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    virtual IUserInput * createUserInputWidget(QWidget * parent = 0);

    virtual QImage * image() const;
    virtual void tagIdChange(int oldId, int newId);
    virtual void tagIdChanged(int oldId, int newId);
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
    void setLocation(const KLocation & loc);
    void setReport(KReport * rep);
    void notifyConnectionsChanged(bool connected);
    void setPortsVisible(bool v);

    explicit IModel(IModelFactory * fact, const KModelInfo& inf);

private:
    Q_DISABLE_COPY(IModel)
    QSharedDataPointer<IModelPrivate> dptr;
};


#endif // IMODEL_H
