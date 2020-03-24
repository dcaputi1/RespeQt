/*
 * optionsdialog.cpp
 *
 * Copyright 2015 Joseph Zatarski
 * Copyright 2016, 2017 TheMontezuma
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#include "optionsdialog.h"
#include "ui_optionsdialog.h"
#include "respeqtsettings.h"
#include "printers/rawoutput.h"
#include <QtSerialPort/QtSerialPort>
#include <QTranslator>
#include <QDir>
#include <QFileDialog>
#include <QFontDialog>
#include <QPrintDialog>

OptionsDialog::OptionsDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::OptionsDialog)
{
    auto flags = windowFlags();
    flags = flags & (~Qt::WindowContextHelpButtonHint);
    setWindowFlags(flags);

    m_ui->setupUi(this);

    m_ui->optionSections->expandAll();
    itemStandard = m_ui->optionSections->topLevelItem(0)->child(0);
    itemAtariSio = m_ui->optionSections->topLevelItem(0)->child(1);
    itemEmulation = m_ui->optionSections->topLevelItem(1);
    itemDiskImages = m_ui->optionSections->topLevelItem(2);
    itemDiskOptions = m_ui->optionSections->topLevelItem(2)->child(0);
    itemDiskOSB = m_ui->optionSections->topLevelItem(2)->child(1);
    itemDiskIcons = m_ui->optionSections->topLevelItem(2)->child(2);
    itemDiskFavorite = m_ui->optionSections->topLevelItem(2)->child(3);
    itemI18n = m_ui->optionSections->topLevelItem(3);
    itemTestSerialPort = m_ui->optionSections->topLevelItem(0)->child(2);
    itemAtari1027 = m_ui->optionSections->topLevelItem(4)->child(0);
    itemPassthrough = m_ui->optionSections->topLevelItem(4)->child(1);

#ifndef Q_OS_LINUX
    m_ui->optionSections->topLevelItem(0)->removeChild(itemAtariSio);
#endif
#ifdef QT_NO_DEBUG
    m_ui->optionSections->topLevelItem(0)->removeChild(itemTestSerialPort);
#endif

    connectSignals();
    setupSettings();
}

void OptionsDialog::setupSettings()
{
    /* Retrieve application settings */
    m_ui->serialPortComboBox->clear();
    const QList<QSerialPortInfo>& infos = QSerialPortInfo::availablePorts();
    for (QList<QSerialPortInfo>::const_iterator it = infos.begin() ; it!=infos.end() ; it++)
    {
        m_ui->serialPortComboBox->addItem(it->portName(),it->systemLocation());
    }
    m_ui->serialPortComboBox->setCurrentText(respeqtSettings->serialPortName());
    if(0 != m_ui->serialPortComboBox->currentText().compare(respeqtSettings->serialPortName(),Qt::CaseInsensitive))
    {
        m_ui->serialPortComboBox->setEditable(true);
        m_ui->serialPortComboBox->addItem(respeqtSettings->serialPortName());
        m_ui->serialPortComboBox->setCurrentText(respeqtSettings->serialPortName());
    }
    else
    {
        m_ui->serialPortComboBox->addItem(tr("Custom"));
    }
    m_ui->serialPortHandshakeCombo->setCurrentIndex(respeqtSettings->serialPortHandshakingMethod());
    m_ui->serialPortFallingEdge->setChecked(respeqtSettings->serialPortTriggerOnFallingEdge());
    m_ui->serialPortDTRControlEnable->setChecked(respeqtSettings->serialPortDTRControlEnable());
    m_ui->serialPortWriteDelayCombo->setCurrentIndex(respeqtSettings->serialPortWriteDelay());
    m_ui->serialPortBaudCombo->setCurrentIndex(respeqtSettings->serialPortMaximumSpeed());
    m_ui->serialPortUseDivisorsBox->setChecked(respeqtSettings->serialPortUsePokeyDivisors());
    m_ui->serialPortDivisorEdit->setValue(respeqtSettings->serialPortPokeyDivisor());
    m_ui->serialPortCompErrDelayBox->setValue(respeqtSettings->serialPortCompErrDelay());
    m_ui->atariSioDriverNameEdit->setText(respeqtSettings->atariSioDriverName());
    m_ui->atariSioHandshakingMethodCombo->setCurrentIndex(respeqtSettings->atariSioHandshakingMethod());
    m_ui->emulationHighSpeedExeLoaderBox->setChecked(respeqtSettings->useHighSpeedExeLoader());
    m_ui->emulationUseCustomCasBaudBox->setChecked(respeqtSettings->useCustomCasBaud());
    m_ui->emulationCustomCasBaudSpin->setValue(respeqtSettings->customCasBaud());
    m_ui->minimizeToTrayBox->setChecked(respeqtSettings->minimizeToTray());
    m_ui->saveWinPosBox->setChecked(respeqtSettings->saveWindowsPos());
    m_ui->saveDiskVisBox->setChecked(respeqtSettings->saveDiskVis());
    m_ui->filterUscore->setChecked(respeqtSettings->filterUnderscore());
    m_ui->capitalLettersPCLINK->setChecked(respeqtSettings->capitalLettersInPCLINK());
    m_ui->URLSubmit->setChecked(respeqtSettings->isURLSubmitEnabled());
    m_ui->spyMode->setChecked(respeqtSettings->isSpyMode());
    m_ui->commandName->setChecked(respeqtSettings->isCommandName());
    m_ui->trackLayout->setChecked(respeqtSettings->isTrackLayout());
    m_ui->disassembleUploadedCode->setChecked(respeqtSettings->disassembleUploadedCode());
    m_ui->translatorDiskImagePath->setText(respeqtSettings->translatorDiskImagePath());
    m_ui->translatorAutomaticDetection->setChecked(respeqtSettings->translatorAutomaticDetection());
    m_ui->hideChipMode->setChecked(respeqtSettings->hideChipMode());
    m_ui->hideHappyMode->setChecked(respeqtSettings->hideHappyMode());
    m_ui->hideNextImage->setChecked(respeqtSettings->hideNextImage());
    m_ui->hideOSBMode->setChecked(respeqtSettings->hideOSBMode());
    m_ui->hideToolDisk->setChecked(respeqtSettings->hideToolDisk());
    m_ui->toolDiskImagePath->setText(respeqtSettings->toolDiskImagePath());
    m_ui->activateChipModeWithTool->setChecked(respeqtSettings->activateChipModeWithTool());
    m_ui->activateHappyModeWithTool->setChecked(respeqtSettings->activateHappyModeWithTool());
    m_ui->useLargerFont->setChecked(respeqtSettings->useLargeFont());
    m_ui->enableShade->setChecked(respeqtSettings->enableShade());
    m_ui->RclNameEdit->setText(respeqtSettings->lastRclDir());
    m_ui->warning_nativemenu->hide();
#ifdef Q_OS_MAC
    m_ui->useNativeMenu->setChecked(respeqtSettings->nativeMenu());
    const auto& actualNoMenu = QApplication::testAttribute(Qt::AA_DontUseNativeMenuBar);
    // The meaning of both flags are the opposite (i.e. boolean not) of each other.
    // So, we have to test for equality to get difference (i.e. a != !b).
    if (actualNoMenu == respeqtSettings->nativeMenu())
        m_ui->warning_nativemenu->show();
    m_ui->minimizeToTrayBox->hide();
#endif

    switch (respeqtSettings->backend()) {
#ifndef QT_NO_DEBUG
        case SERIAL_BACKEND_TEST:
#endif
        case SERIAL_BACKEND_STANDARD:
            itemStandard->setCheckState(0, Qt::Checked);
            itemAtariSio->setCheckState(0, Qt::Unchecked);
#ifndef QT_NO_DEBUG
            itemTestSerialPort->setCheckState(0, Qt::Unchecked);
#endif
            m_ui->optionSections->setCurrentItem(itemStandard);
            break;
        case SERIAL_BACKEND_SIO_DRIVER:
            itemStandard->setCheckState(0, Qt::Unchecked);
            itemAtariSio->setCheckState(0, Qt::Checked);
#ifndef QT_NO_DEBUG
            itemTestSerialPort->setCheckState(0, Qt::Unchecked);
#endif
            m_ui->optionSections->setCurrentItem(itemAtariSio);
            break;
    }
    m_ui->serialPortBox->setCheckState(itemStandard->checkState(0));
    m_ui->atariSioBox->setCheckState(itemAtariSio->checkState(0));

    /* list available translations */
    QTranslator local_translator;
    m_ui->i18nLanguageCombo->clear();
    m_ui->i18nLanguageCombo->addItem(tr("Automatic"), "auto");
    if (respeqtSettings->i18nLanguage().compare("auto") == 0)
      m_ui->i18nLanguageCombo->setCurrentIndex(0);
    m_ui->i18nLanguageCombo->addItem(QT_TR_NOOP("English"), "en");
    if (respeqtSettings->i18nLanguage().compare("en") == 0)
      m_ui->i18nLanguageCombo->setCurrentIndex(1);
    QDir dir(":/translations/i18n/");
    QStringList filters;
    filters << "respeqt_*.qm";
    dir.setNameFilters(filters);
    for (int i = 0; i < dir.entryList().size(); ++i) {
        local_translator.load(":/translations/i18n/" + dir.entryList()[i]);
        m_ui->i18nLanguageCombo->addItem(local_translator.translate("OptionsDialog", "English"), dir.entryList()[i].replace("respeqt_", "").replace(".qm", ""));
        if (dir.entryList()[i].replace("respeqt_", "").replace(".qm", "").compare(respeqtSettings->i18nLanguage()) == 0) {
            m_ui->i18nLanguageCombo->setCurrentIndex(i+2);
        }
    }

    bool software_handshake = (respeqtSettings->serialPortHandshakingMethod()==HANDSHAKE_SOFTWARE);
    m_ui->serialPortWriteDelayLabel->setVisible(software_handshake);
    m_ui->serialPortWriteDelayCombo->setVisible(software_handshake);
    m_ui->serialPortBaudLabel->setVisible(!software_handshake);
    m_ui->serialPortBaudCombo->setVisible(!software_handshake);
    m_ui->serialPortUseDivisorsBox->setVisible(!software_handshake);
    m_ui->serialPortDivisorLabel->setVisible(!software_handshake);
    m_ui->serialPortDivisorEdit->setVisible(!software_handshake);
    m_ui->serialPortCompErrDelayLabel->setVisible(!software_handshake);
    m_ui->serialPortCompErrDelayBox->setVisible(!software_handshake);
#ifdef Q_OS_WIN
    bool no_handshake = (respeqtSettings->serialPortHandshakingMethod()==HANDSHAKE_NO_HANDSHAKE);
    m_ui->serialPortFallingEdge->setVisible(!no_handshake && !software_handshake);
    m_ui->serialPortDTRControlEnable->setVisible(no_handshake || software_handshake);
#else
    m_ui->serialPortFallingEdge->setVisible(false);
#endif

    if((SERIAL_BACKEND_STANDARD == respeqtSettings->backend()) && software_handshake)
    {
        m_ui->emulationHighSpeedExeLoaderBox->setVisible(false);
    }
    else
    {
        m_ui->emulationHighSpeedExeLoaderBox->setVisible(true);
    }

    m_ui->label_atarifixed->setText(respeqtSettings->atariFixedFontFamily());
    QFont font;
    font.setPointSize(12);
    font.setFamily(m_ui->label_atarifixed->text());
    m_ui->fontSample->setFont(font);

#ifdef Q_OS_MAC
    m_ui->useNativeMenu->show();
    m_ui->useNativeMenu->setChecked(respeqtSettings->nativeMenu());
#else
    m_ui->useNativeMenu->hide();
#endif
    // Setup via platform dependent class
    Printers::RawOutput::setupRawPrinters(m_ui->rawPrinterName);
    QString rawPrinterName = respeqtSettings->rawPrinterName();
    if (rawPrinterName.length() > 0)
        m_ui->rawPrinterName->setCurrentText(rawPrinterName);
}

// This will connect all necessary signals to slots on this dialog.
void OptionsDialog::connectSignals()
{
    // Dialog
    connect(this, &OptionsDialog::accepted, this, &OptionsDialog::saveSettings);

    // Tree
    connect(m_ui->optionSections, &QTreeWidget::itemClicked, this, &OptionsDialog::sectionClicked);
    connect(m_ui->optionSections, &QTreeWidget::currentItemChanged, this, &OptionsDialog::currentSectionChanged);

    // Serial port section
    void (QComboBox::*serialPortChanged)(int) = &QComboBox::currentIndexChanged;
    connect(m_ui->serialPortComboBox, serialPortChanged, this, &OptionsDialog::serialPortChanged);
    void (QComboBox::*handshakeChanged)(int) = &QComboBox::currentIndexChanged;
    connect(m_ui->serialPortHandshakeCombo, handshakeChanged, this, &OptionsDialog::handshakeChanged);
    connect(m_ui->serialPortUseDivisorsBox, &QCheckBox::toggled, this, &OptionsDialog::useDivisorToggled);

#ifndef QT_NO_QDEBUG
    // Serial test section
    connect(m_ui->testFileButton, &QPushButton::clicked, this, &OptionsDialog::testFileClicked);
#endif

    // Emulation section
    connect(m_ui->emulationUseCustomCasBaudBox, &QCheckBox::toggled, this, &OptionsDialog::useCustomBaudToggled);
    connect(m_ui->buttonRclFolder, &QPushButton::clicked, this, &OptionsDialog::rclFolderClicked);

    // Disk images
#ifdef SHOWFIRMWARE
    connect(m_ui->atari810FirmwareButton, &QPushButton::clicked, this, &OptionsDialog::select810FirmwareTriggered);
    connect(m_ui->atari810HappyFirmwareButton, &QPushButton::clicked, this, &OptionsDialog::select810HappyFirmwareTriggered);
    connect(m_ui->atari810ChipFirmwareButton, &QPushButton::clicked, this, &OptionsDialog::select810ChipFirmwareTriggered);
    connect(m_ui->atari1050FirmwareButton, &QPushButton::clicked, this, &OptionsDialog::select1050FirmwareTriggered);
    connect(m_ui->atari1050HappyFirmwareButton, &QPushButton::clicked, this, &OptionsDialog::select1050HappyFirmwareTriggered);
    connect(m_ui->atari1050TurboFirmwareButton, &QPushButton::clicked, this, &OptionsDialog::select1050TurboFirmwareTriggered);
    connect(m_ui->atari1050SpeedyFirmwareButton, &QPushButton::clicked, this, &OptionsDialog::select1050SpeedyFirmwareTriggered);
    connect(m_ui->atari1050ArchiverFirmwareButton, &QPushButton::clicked, this, &OptionsDialog::select1050ArchiverFirmwareTriggered);
    connect(m_ui->atari1050DuplicatorFirmwareButton, &QPushButton::clicked, this, &OptionsDialog::select1050DuplicatorFirmwareTriggered);
#endif
    connect(m_ui->toolDiskSelectButton, &QPushButton::clicked, this, &OptionsDialog::selectToolDiskTriggered);
    connect(m_ui->translatorSelectButton, &QPushButton::clicked, this, &OptionsDialog::selectTranslatorDiskTriggered);

    // printer section
    connect(m_ui->button_atarifixed, &QPushButton::clicked, this, &OptionsDialog::fixedFontClicked);

    // UI section
    connect(m_ui->useNativeMenu, &QCheckBox::toggled, this, &OptionsDialog::useNativeMenuToggled);
}

void OptionsDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void OptionsDialog::serialPortChanged(int index)
{
    bool isCustomPath = !m_ui->serialPortComboBox->itemData(index).isValid();
    m_ui->serialPortComboBox->setEditable(isCustomPath);
}

void OptionsDialog::handshakeChanged(int index)
{
    auto software_handshake = (index==HANDSHAKE_SOFTWARE);
    m_ui->serialPortWriteDelayLabel->setVisible(software_handshake);
    m_ui->serialPortWriteDelayCombo->setVisible(software_handshake);
    m_ui->serialPortBaudLabel->setVisible(!software_handshake);
    m_ui->serialPortBaudCombo->setVisible(!software_handshake);
    m_ui->serialPortUseDivisorsBox->setVisible(!software_handshake);
    m_ui->serialPortDivisorLabel->setVisible(!software_handshake);
    m_ui->serialPortDivisorEdit->setVisible(!software_handshake);
    m_ui->serialPortCompErrDelayLabel->setVisible(!software_handshake);
    m_ui->serialPortCompErrDelayBox->setVisible(!software_handshake);
#ifdef Q_OS_WIN
    bool no_handshake = (index==HANDSHAKE_NO_HANDSHAKE);
    m_ui->serialPortFallingEdge->setVisible(!no_handshake && !software_handshake);
    m_ui->serialPortDTRControlEnable->setVisible(no_handshake || software_handshake);
#endif
    if(itemStandard->checkState((0)) == Qt::Checked)
    {
        m_ui->emulationHighSpeedExeLoaderBox->setVisible(!software_handshake);
    }
}

void OptionsDialog::useDivisorToggled(bool checked)
{
    m_ui->serialPortBaudLabel->setEnabled(!checked);
    m_ui->serialPortBaudCombo->setEnabled(!checked);
    m_ui->serialPortDivisorLabel->setEnabled(checked);
    m_ui->serialPortDivisorEdit->setEnabled(checked);
}

// TODO Bug with serial selection
void OptionsDialog::sectionClicked(QTreeWidgetItem* item, int column)
{
    if (item->checkState(column) == Qt::Checked)
    {
        if (item == itemStandard)
        {
            m_ui->emulationHighSpeedExeLoaderBox->setVisible(HANDSHAKE_SOFTWARE != m_ui->serialPortHandshakeCombo->currentIndex());
        }
        else
        {
            itemStandard->setCheckState(column, Qt::Unchecked);
        }
        if (item == itemAtariSio)
        {
            m_ui->emulationHighSpeedExeLoaderBox->setVisible(true);
        }
        else
        {
            itemAtariSio->setCheckState(column, Qt::Unchecked);
        }
#ifndef QT_NO_DEBUG
        if (item != itemTestSerialPort)
        {
            itemTestSerialPort->setCheckState(column, Qt::Unchecked);
        }
#endif
    }
    else if ((itemStandard->checkState(column) == Qt::Unchecked) &&
            (itemAtariSio->checkState(column) == Qt::Unchecked))
    {
        item->setCheckState(column, Qt::Checked);
    }
    m_ui->serialPortBox->setCheckState(itemStandard->checkState(column));
    m_ui->atariSioBox->setCheckState(itemAtariSio->checkState(column));
#ifndef QT_NO_DEBUG
    m_ui->serialTestBox->setCheckState(itemTestSerialPort->checkState(column));
#endif
}

void OptionsDialog::currentSectionChanged(QTreeWidgetItem* current, QTreeWidgetItem* /*previous*/)
{
    if (current == itemStandard) {
        m_ui->stackedWidget->setCurrentIndex(0);
    } else if (current == itemAtariSio) {
        m_ui->stackedWidget->setCurrentIndex(1);
    } else if (current == itemTestSerialPort) {
        m_ui->stackedWidget->setCurrentIndex(2);
    } else if (current == itemEmulation) {
        m_ui->stackedWidget->setCurrentIndex(3);
    } else if (current == itemDiskOptions) {
        m_ui->stackedWidget->setCurrentIndex(4);
    } else if (current == itemDiskOSB) {
        m_ui->stackedWidget->setCurrentIndex(12);
    } else if (current == itemDiskIcons) {
        m_ui->stackedWidget->setCurrentIndex(13);
    } else if (current == itemDiskFavorite) {
        m_ui->stackedWidget->setCurrentIndex(14);
    } else if (current == itemI18n) {
        m_ui->stackedWidget->setCurrentIndex(5);
    } else if (current == itemAtari1027) {
        m_ui->stackedWidget->setCurrentIndex(6);
    } else if (current == itemPassthrough) {
        m_ui->stackedWidget->setCurrentIndex(7);
    }
}

void OptionsDialog::saveSettings()
{
    respeqtSettings->setSerialPortName(m_ui->serialPortComboBox->currentText());
    respeqtSettings->setSerialPortHandshakingMethod(m_ui->serialPortHandshakeCombo->currentIndex());
    respeqtSettings->setSerialPortTriggerOnFallingEdge(m_ui->serialPortFallingEdge->isChecked());
    respeqtSettings->setSerialPortDTRControlEnable(m_ui->serialPortDTRControlEnable->isChecked());
    respeqtSettings->setSerialPortWriteDelay(m_ui->serialPortWriteDelayCombo->currentIndex());
    respeqtSettings->setSerialPortCompErrDelay(m_ui->serialPortCompErrDelayBox->value());
    respeqtSettings->setSerialPortMaximumSpeed(m_ui->serialPortBaudCombo->currentIndex());
    respeqtSettings->setSerialPortUsePokeyDivisors(m_ui->serialPortUseDivisorsBox->isChecked());
    respeqtSettings->setSerialPortPokeyDivisor(m_ui->serialPortDivisorEdit->value());
    respeqtSettings->setAtariSioDriverName(m_ui->atariSioDriverNameEdit->text());
    respeqtSettings->setAtariSioHandshakingMethod(m_ui->atariSioHandshakingMethodCombo->currentIndex());
    respeqtSettings->setUseHighSpeedExeLoader(m_ui->emulationHighSpeedExeLoaderBox->isChecked());
    respeqtSettings->setUseCustomCasBaud(m_ui->emulationUseCustomCasBaudBox->isChecked());
    respeqtSettings->setCustomCasBaud(m_ui->emulationCustomCasBaudSpin->value());
    respeqtSettings->setMinimizeToTray(m_ui->minimizeToTrayBox->isChecked());
    respeqtSettings->setsaveWindowsPos(m_ui->saveWinPosBox->isChecked());
    respeqtSettings->setsaveDiskVis(m_ui->saveDiskVisBox->isChecked());
    respeqtSettings->setfilterUnderscore(m_ui->filterUscore->isChecked());
    respeqtSettings->setCapitalLettersInPCLINK(m_ui->capitalLettersPCLINK->isChecked());
    respeqtSettings->setURLSubmit(m_ui->URLSubmit->isChecked());
    respeqtSettings->setSpyMode(m_ui->spyMode->isChecked());
    respeqtSettings->setCommandName(m_ui->commandName->isChecked());
    respeqtSettings->setTrackLayout(m_ui->trackLayout->isChecked());
    respeqtSettings->setDisassembleUploadedCode(m_ui->disassembleUploadedCode->isChecked());
    respeqtSettings->setTranslatorAutomaticDetection(m_ui->translatorAutomaticDetection->isChecked());
    respeqtSettings->setTranslatorDiskImagePath(m_ui->translatorDiskImagePath->text());
    respeqtSettings->setHideChipMode(m_ui->hideChipMode->isChecked());
    respeqtSettings->setHideHappyMode(m_ui->hideHappyMode->isChecked());
    respeqtSettings->setHideNextImage(m_ui->hideNextImage->isChecked());
    respeqtSettings->setHideOSBMode(m_ui->hideOSBMode->isChecked());
    respeqtSettings->setHideToolDisk(m_ui->hideToolDisk->isChecked());
    respeqtSettings->setToolDiskImagePath(m_ui->toolDiskImagePath->text());
    respeqtSettings->setActivateChipModeWithTool(m_ui->activateChipModeWithTool->isChecked());
    respeqtSettings->setActivateHappyModeWithTool(m_ui->activateHappyModeWithTool->isChecked());
    respeqtSettings->setUseLargeFont(m_ui->useLargerFont->isChecked());
    respeqtSettings->setEnableShade(m_ui->enableShade->isChecked());
    respeqtSettings->setRclDir(m_ui->RclNameEdit->text());

    int backend = SERIAL_BACKEND_STANDARD;
    if (itemAtariSio->checkState(0) == Qt::Checked)
    {
        backend = SERIAL_BACKEND_SIO_DRIVER;
    }
#ifndef QT_NO_DEBUG
    else if (itemTestSerialPort->checkState(0) == Qt::Checked)
    {
        backend = SERIAL_BACKEND_TEST;
    }
#endif

    respeqtSettings->setBackend(backend);

    respeqtSettings->setI18nLanguage(m_ui->i18nLanguageCombo->itemData(m_ui->i18nLanguageCombo->currentIndex()).toString());
#ifdef Q_OS_MAC
    respeqtSettings->setNativeMenu(m_ui->useNativeMenu->isChecked());
#endif
    if (m_ui->rawPrinterName->currentData() != -1)
        respeqtSettings->setRawPrinterName(m_ui->rawPrinterName->currentText());
    else
        respeqtSettings->setRawPrinterName("");
}

void OptionsDialog::useCustomBaudToggled(bool checked)
{
    m_ui->emulationCustomCasBaudSpin->setEnabled(checked);
}

void OptionsDialog::selectFirmware(QLineEdit *edit, QString title, QString filters)
{
    auto dir = edit->text();
    auto lastSlash = dir.lastIndexOf("/");
    auto lastBackslash = dir.lastIndexOf("\\");
    if ((lastSlash != -1) || (lastBackslash != -1))
    {
        auto lastIndex = lastSlash > lastBackslash ? lastSlash : lastBackslash;
        dir = dir.left(lastIndex);
    }
    else
    {
        dir = "";
    }
    auto fileName = QFileDialog::getOpenFileName(this, title, dir, filters);
    if (fileName.isEmpty()) {
        return;
    }
    edit->setText(fileName);
}

#ifdef SHOWFIRMWARE
void OptionsDialog::select810FirmwareTriggered()
{
    selectFirmware(m_ui->atari810FirmwarePath, tr("Select Atari 810 firmware"), tr("Atari drive firmware (*.rom);;All files (*)"));
}

void OptionsDialog::select810ChipFirmwareTriggered()
{
    selectFirmware(m_ui->atari810ChipFirmwarePath, tr("Select Atari 810 Chip firmware"), tr("Atari drive firmware (*.rom);;All files (*)"));
}

void OptionsDialog::select810HappyFirmwareTriggered()
{
    selectFirmware(m_ui->atari810HappyFirmwarePath, tr("Select Atari 810 Happy firmware"), tr("Atari drive firmware (*.rom);;All files (*)"));
}

void OptionsDialog::select1050FirmwareTriggered()
{
    selectFirmware(m_ui->atari1050FirmwarePath, tr("Select Atari 1050 firmware"), tr("Atari drive firmware (*.rom);;All files (*)"));
}

void OptionsDialog::select1050ArchiverFirmwareTriggered()
{
    selectFirmware(m_ui->atari1050ArchiverFirmwarePath, tr("Select Atari 1050 Archiver firmware"), tr("Atari drive firmware (*.rom);;All files (*)"));
}

void OptionsDialog::select1050HappyFirmwareTriggered()
{
    selectFirmware(m_ui->atari1050HappyFirmwarePath, tr("Select Atari 1050 Happy firmware"), tr("Atari drive firmware (*.rom);;All files (*)"));
}

void OptionsDialog::select1050SpeedyFirmwareTriggered()
{
    selectFirmware(m_ui->atari1050SpeedyFirmwarePath, tr("Select Atari 1050 Speedy firmware"), tr("Atari drive firmware (*.rom);;All files (*)"));
}

void OptionsDialog::select1050TurboFirmwareTriggered()
{
    selectFirmware(m_ui->atari1050TurboFirmwarePath, tr("Select Atari 1050 Turbo firmware"), tr("Atari drive firmware (*.rom);;All files (*)"));
}

void OptionsDialog::select1050DuplicatorFirmwareTriggered()
{
    selectFirmware(m_ui->atari1050DuplicatorFirmwarePath, tr("Select Atari 1050 Duplicator firmware"), tr("Atari drive firmware (*.rom);;All files (*)"));
}
#endif

void OptionsDialog::selectTranslatorDiskTriggered()
{
    selectFirmware(m_ui->translatorDiskImagePath, tr("Select translator disk image"), tr("Atari disk image (*.atr);;All files (*)"));
}

void OptionsDialog::selectToolDiskTriggered()
{
    selectFirmware(m_ui->toolDiskImagePath, tr("Select tool disk image"), tr("Atari disk image (*.atr);;All files (*)"));
}

void OptionsDialog::testFileClicked()
{
#ifndef QT_NO_DEBUG
    auto file1Name = QFileDialog::getOpenFileName(this,
             tr("Open test XML File"), QString(), tr("XML Files (*.xml)"));
    m_ui->testFileLabel->setText(file1Name);
    respeqtSettings->setTestFile(file1Name);
#endif
}

void OptionsDialog::fixedFontClicked()
{
    bool ok;
    QFont font;
    font.setFamily(m_ui->label_atarifixed->text());
    QFontDialog::FontDialogOptions options = QFontDialog::MonospacedFonts;
    QFont newFont = QFontDialog::getFont(&ok, font, this, tr("Select Atari fixed width font"), options);
    if (ok)
    {
        newFont.setPointSize(12);
        m_ui->label_atarifixed->setText(newFont.family());
        m_ui->fontSample->setFont(newFont);
        respeqtSettings->setAtariFixedFontFamily(newFont.family());
    }
}

void OptionsDialog::rclFolderClicked()
{
     QString dir;
     dir = respeqtSettings->lastRclDir();
     QString fileName = QFileDialog::getExistingDirectory(this, tr("Selec RCL image folder"), dir);
     fileName = QDir::fromNativeSeparators(fileName);    //
     if (fileName.isEmpty()) {
         return;
     }
     respeqtSettings->setRclDir(fileName);
     m_ui->RclNameEdit->setText(fileName);

}

void OptionsDialog::useNativeMenuToggled()
{
    // The meaning of both flags are the opposite (i.e. boolean not) of each other.
    // So, we have to test for equality to get difference (i.e. a != !b).
    const auto& actualNoMenu = QApplication::testAttribute(Qt::AA_DontUseNativeMenuBar);
    // We reverse the meaning of the checkbox to match the application attribute, see above for reason.
    const auto& checkboxNoMenu = m_ui->useNativeMenu->checkState() == Qt::Unchecked;
    m_ui->warning_nativemenu->setVisible(actualNoMenu != checkboxNoMenu);
}
