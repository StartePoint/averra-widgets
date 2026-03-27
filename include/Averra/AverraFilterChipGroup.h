#ifndef AVERRA_FILTER_CHIP_GROUP_H
#define AVERRA_FILTER_CHIP_GROUP_H

#include <Averra/AverraGlobal.h>

#include <QWidget>

class QButtonGroup;
class QPushButton;
class QHBoxLayout;

class AVERRA_EXPORT AverraFilterChipGroup : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool multiSelect READ multiSelect WRITE setMultiSelect NOTIFY multiSelectChanged)

public:
    explicit AverraFilterChipGroup(QWidget *parent = nullptr);

    bool multiSelect() const;
    void setMultiSelect(bool multiSelect);

    int chipCount() const;
    QString chipText(int index) const;
    QList<int> selectedIndices() const;
    void setSelectedIndices(const QList<int> &indices);

    void addChip(const QString &text);
    void clearChips();

signals:
    void multiSelectChanged(bool multiSelect);
    void selectedIndicesChanged(const QList<int> &indices);
    void chipClicked(int index, bool selected);

private slots:
    void initialize();
    void refreshStyle();
    void handleButtonClicked(int id);

private:
    void refreshSelectionState();

    bool m_multiSelect;
    QButtonGroup *m_buttonGroup;
    QHBoxLayout *m_layout;
    QList<QPushButton *> m_buttons;
};

#endif // AVERRA_FILTER_CHIP_GROUP_H

