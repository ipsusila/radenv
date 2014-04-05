#include "uiautorowtable.h"

UiAutoRowTable::UiAutoRowTable(QWidget *parent) :
    QTableWidget(parent)
{
    connect(this, SIGNAL(cellChanged(int,int)), this, SLOT(onCellChanged(int,int)));
}
UiAutoRowTable::~UiAutoRowTable()
{
}
void UiAutoRowTable::changeCell(int row, int column)
{
    if (column == 0) {
        bool lastRow = (row + 1) == this->rowCount();
        QTableWidgetItem * item = this->item(row, column);
        if (lastRow) {
            if (!item->text().isEmpty())
                this->setRowCount(row+2);
        }
        else {
            if (item->text().isEmpty()) {
                this->removeRow(row);
            }
        }
    }
}

void UiAutoRowTable::onCellChanged(int row, int column)
{
    changeCell(row, column);
}
