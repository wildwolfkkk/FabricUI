//
// Copyright (c) 2010-2016, Fabric Software Inc. All rights reserved.
//

#ifndef __UI_DFG_DFGController__
#define __UI_DFG_DFGController__

#include <FabricUI/DFG/DFGBindingNotifier.h>
#include <FabricUI/GraphView/Controller.h>
#include <FabricUI/GraphView/Node.h>
#include <FabricUI/GraphView/Pin.h>
#include <FabricUI/GraphView/Port.h>
#include <FabricUI/GraphView/BackDropNode.h>
#include <FabricUI/ValueEditor_Legacy/ValueItem.h>
#include <SplitSearch/SplitSearch.hpp>
#include <vector>
#include <ASTWrapper/KLASTManager.h>

using namespace FabricUI::ValueEditor_Legacy;

namespace FabricUI
{

  namespace DFG
  {

    class DFGUICmdHandler;
    class DFGNotificationRouter;
    class DFGWidget;

    class DFGController : public FabricUI::GraphView::Controller
    {
      Q_OBJECT

      friend class DFGValueEditor;
      friend class DFGWidget;

    public:

      typedef void(*LogFunc)(const char * message);

      DFGController(
        GraphView::Graph * graph,
        DFGWidget *dfgWidget,
        FabricCore::Client &client,
        FabricServices::ASTWrapper::KLASTManager * manager,
        DFGUICmdHandler *cmdHandler,
        bool overTakeBindingNotifications = true
        );
      ~DFGController();

      DFGWidget *getDFGWidget() const
        { return m_dfgWidget; }
      
      FabricCore::Client &getClient()
        { return m_client; }
      FabricCore::DFGHost &getHost()
        { return m_host; }
      FabricCore::DFGBinding &getBinding()
        { return m_binding; }
      QSharedPointer<DFGBindingNotifier> const &
      getBindingNotifier()
        { return m_bindingNotifier; }
      FTL::CStrRef getExecPath()
        { return m_execPath; }
      QString getExecPath_QS()
        { return QString::fromUtf8( m_execPath.data(), m_execPath.size() ); }
      FabricCore::DFGExec &getExec()
        { return m_exec; }

      DFGUICmdHandler *getCmdHandler() const
        { return m_cmdHandler; }

      void setHostBindingExec(
        FabricCore::DFGHost &host,
        FabricCore::DFGBinding &binding,
        FTL::StrRef execPath,
        FabricCore::DFGExec &exec
        );
      void setBindingExec(
        FabricCore::DFGBinding &binding,
        FTL::StrRef execPath,
        FabricCore::DFGExec &exec
        );
      void setExec(
        FTL::StrRef execPath,
        FabricCore::DFGExec &exec
        );
      void refreshExec();

      void focusNode( FTL::StrRef nodeName );

      DFGNotificationRouter * getRouter();
      void setRouter(DFGNotificationRouter * router);

      bool isViewingRootGraph() const
        { return m_execPath.empty(); }
      FabricServices::ASTWrapper::KLASTManager * astManager()
        { return m_manager; }

      // returns true if the user allows a preset edit
      bool validPresetSplit() const;

      // Parent virtual functions

      virtual bool gvcDoRemoveNodes(
        FTL::ArrayRef<GraphView::Node *> nodes
        );

      virtual bool gvcDoAddConnection(
        GraphView::ConnectionTarget * src,
        GraphView::ConnectionTarget * dst
        );

      virtual bool gvcDoRemoveConnection(
        GraphView::ConnectionTarget * src,
        GraphView::ConnectionTarget * dst
        );
      
      virtual bool gvcDoAddInstFromPreset(
        QString presetPath,
        QPointF pos
        );

      virtual void gvcDoAddPort(
        QString desiredPortName,
        GraphView::PortType portType,
        QString typeSpec = QString(),
        GraphView::ConnectionTarget *connectWith = 0,
        QString extDep = QString(),
        QString metaData = QString()
        );

      virtual void gvcDoSetNodeCommentExpanded(
        GraphView::Node *node,
        bool expanded
        );

      virtual void gvcDoMoveNodes(
        std::vector<GraphView::Node *> const &nodes,
        QPointF delta,
        bool allowUndo
        );

      virtual void gvcDoResizeBackDropNode(
        GraphView::BackDropNode *backDropNode,
        QPointF newTopLeftPos,
        QSizeF newSize,
        bool allowUndo
        );

      // Commands

      void cmdRemoveNodes(
        QStringList nodeNames
        );

      void cmdConnect(
        QString srcPath, 
        QString dstPath
        );

      void cmdDisconnect(
        QString srcPath, 
        QString dstPath
        );

      QString cmdAddInstWithEmptyGraph(
        QString title,
        QPointF pos
        );

      QString cmdAddInstWithEmptyFunc(
        QString title,
        QString initialCode,
        QPointF pos
        );

      QString cmdAddInstFromPreset(
        QString presetPath,
        QPointF pos
        );

      QString cmdAddVar(
        QString desiredNodeName,
        QString dataType,
        QString extDep,
        QPointF pos
        );

      QString cmdAddGet(
        QString desiredNodeName,
        QString varPath,
        QPointF pos
        );

      QString cmdAddSet(
        QString desiredNodeName,
        QString varPath,
        QPointF pos
        );

      QString cmdAddPort(
        QString desiredPortName,
        FabricCore::DFGPortType dfgPortType,
        QString typeSpec,
        QString portToConnect,
        QString extDep,
        QString uiMetadata
        );
      
      QString cmdCreatePreset(
        QString nodeName,
        QString presetDirPath,
        QString presetName
        );

      QString cmdEditPort(
        QString oldPortName,
        QString desiredNewPortName,
        QString typeSpec,
        QString extDep,
        QString uiMetadata
        );

      void cmdRemovePort(
        QString portName
        );

      void cmdMoveNodes(
        QStringList nodeNames,
        QList<QPointF> newTopLeftPoss
        );

      void cmdResizeBackDropNode(
        QString backDropNodeName,
        QPointF newTopLeftPos,
        QSizeF newSize
        );

      QString cmdImplodeNodes(
        QStringList nodeNames,
        QString desiredNodeName
        );

      QList<QString> cmdExplodeNode(
        QString nodeName
        );

      void cmdAddBackDrop(
        QString title,
        QPointF pos
        );

      void cmdSetNodeComment(
        QString nodeName, 
        QString comment
        );

      void setNodeCommentExpanded(
        QString nodeName, 
        bool expanded
        );

      void cmdSetCode( QString code );

      QString cmdEditNode(
        QString oldName,
        QString desiredNewName,
        QString nodeMetadata,
        QString execMetadata
        );

      QString cmdRenameExecPort(
        QString oldName,
        QString desiredNewName
        );

      void cmdCut();

      void cmdPaste();

      void cmdSetArgValue(
        QString argName,
        FabricCore::RTVal const &value
        );

      bool cmdSetDefaultValue(
        FabricCore::DFGBinding &binding,
        QString execPath,
        FabricCore::DFGExec &exec,
        QString portPath,
        FabricCore::RTVal const &value
        );

      void cmdSetRefVarPath(
        FabricCore::DFGBinding &binding,
        QString execPath,
        FabricCore::DFGExec &exec,
        QString refName,
        QString varPath
        );

      void cmdReorderPorts(
        FabricCore::DFGBinding &binding,
        QString execPath,
        FabricCore::DFGExec &exec,
        QList<int> indices
        );

      void cmdSetExtDeps(
        QStringList nameAndVers
        );

      void cmdSplitFromPreset();

      virtual QString reloadCode();

      virtual bool zoomCanvas(float zoom);
      virtual bool panCanvas(QPointF pan);
      virtual bool relaxNodes(QStringList paths = QStringList());
      virtual bool setNodeColor(const char * nodeName, const char * key, QColor color);
      /// Sets the collapse state of the selected node.
      /// Saves it in the node preferences    
      virtual void setSelectedNodeCollapseState(int collapseState);
      
      virtual std::string copy();

      virtual bool reloadExtensionDependencies(char const * path);

      virtual void log(const char * message);
      virtual void logError(const char * message);

      virtual void setLogFunc(LogFunc func);

      void execute();

      virtual bool canConnectTo(
        char const *pathA,
        char const *pathB,
        std::string &failureReason
        );

      virtual QStringList getPresetPathsFromSearch(char const * search, bool includePresets = true, bool includeNameSpaces = false);

      virtual DFGNotificationRouter *createRouter();

      static QStringList getVariableWordsFromBinding(FabricCore::DFGBinding & binding, FTL::CStrRef currentExecPath);

      void emitVarsChanged()
      {
        if ( m_updateSignalBlockCount > 0 )
          m_varsChangedPending = true;
        else
          emit varsChanged();
      }

      void emitArgsChanged()
      {
        if ( m_updateSignalBlockCount > 0 )
          m_argsChangedPending = true;
        else
          emit argsChanged();
      }

      void emitArgValuesChanged()
      {
        if (m_updateSignalBlockCount > 0)
          m_argValuesChangedPending = true;
        else
          emit argValuesChanged();
      }

      void emitDefaultValuesChanged()
      {
        if ( m_updateSignalBlockCount > 0 )
          m_defaultValuesChangedPending = true;
        else
          emit defaultValuesChanged();
      }

      void emitTopoDirty()
      {
        if ( m_updateSignalBlockCount > 0 )
          m_topoDirtyPending = true;
        else
          emit topoDirty();
      }

      void emitDirty()
      {
        if ( m_updateSignalBlockCount > 0 )
          m_dirtyPending = true;
        else
          emit dirty();
      }

      void emitExecSplitChanged()
      {
        emit execSplitChanged();
      }

      void setBlockCompilations( bool blockCompilations );

      class UpdateSignalBlocker
      {
      public:

        UpdateSignalBlocker( DFGController *controller )
          : m_controller( controller )
        {
          ++m_controller->m_updateSignalBlockCount;
        }

        ~UpdateSignalBlocker()
        {
          if ( --m_controller->m_updateSignalBlockCount == 0 )
          {
            if ( m_controller->m_varsChangedPending )
            {
              m_controller->m_varsChangedPending = false;
              emit m_controller->varsChanged();
            }
            if ( m_controller->m_argsChangedPending )
            {
              m_controller->m_argsChangedPending = false;
              emit m_controller->argsChanged();
            }
            if ( m_controller->m_argValuesChangedPending )
            {
              m_controller->m_argValuesChangedPending = false;
              emit m_controller->argValuesChanged();
            }
            if ( m_controller->m_defaultValuesChangedPending )
            {
              m_controller->m_defaultValuesChangedPending = false;
              emit m_controller->defaultValuesChanged();
            }
            if ( m_controller->m_topoDirtyPending )
            {
              m_controller->m_topoDirtyPending = false;
              emit m_controller->topoDirty();

              // [pzion 20160405] FE-6269
              // topoDirty implies dirty
              m_controller->m_dirtyPending = false;
            }
            if ( m_controller->m_dirtyPending )
            {
              m_controller->m_dirtyPending = false;
              emit m_controller->dirty();
            }
          }
        }

      private:

        DFGController *m_controller;
      };

      void emitNodeRenamed(
        FTL::CStrRef oldNodeName,
        FTL::CStrRef newNodeName
        );
      void emitNodeRemoved( FTL::CStrRef nodeName );

      void updateNodeErrors();

    signals:

      void hostChanged();
      void bindingChanged( FabricCore::DFGBinding const &newBinding );
      void execChanged();

      void varsChanged();
      void argsChanged();
      void argInserted( int index, const char* name, const char* type );
      void argTypeChanged( int index, const char* name, const char* type );
      void argRemoved( int index, const char* name );
      void argsReordered( const FTL::JSONArray* newOrder );

      void argValuesChanged();
      void defaultValuesChanged();
      void topoDirty();
      void dirty();
      void execSplitChanged();

      void nodeEditRequested(FabricUI::GraphView::Node *);

      void nodeRenamed(
        FTL::CStrRef execPath,
        FTL::CStrRef oldNodeName,
        FTL::CStrRef newNodeName 
        );
      void nodeRemoved(
        FTL::CStrRef execPath,
        FTL::CStrRef nodeName
        );

    public slots:

      void onTopoDirty();

      void onValueItemDelta( ValueItem *valueItem );
      void onValueItemInteractionEnter( ValueItem *valueItem );
      void onValueItemInteractionDelta( ValueItem *valueItem );
      void onValueItemInteractionLeave( ValueItem *valueItem );

      void onVariablesChanged();
      virtual void onNodeHeaderButtonTriggered(FabricUI::GraphView::NodeHeaderButton * button);

    private:

      void updateErrors();
      void updatePresetPathDB();

      DFGWidget *m_dfgWidget;
      FabricCore::Client m_client;
      FabricCore::DFGHost m_host;
      FabricCore::DFGBinding m_binding;
      QSharedPointer<DFGBindingNotifier> m_bindingNotifier;
      std::string m_execPath;
      FabricCore::DFGExec m_exec;
      FabricServices::ASTWrapper::KLASTManager * m_manager;
      DFGUICmdHandler *m_cmdHandler;
      DFGNotificationRouter * m_router;
      LogFunc m_logFunc;
      bool const m_overTakeBindingNotifications;
      FabricServices::SplitSearch::Dict m_presetNameSpaceDict;
      FabricServices::SplitSearch::Dict m_presetPathDict;
      std::vector<std::string> m_presetNameSpaceDictSTL;
      std::vector<std::string> m_presetPathDictSTL;
      bool m_presetDictsUpToDate;

      uint32_t m_updateSignalBlockCount;
      bool m_varsChangedPending;
      bool m_argsChangedPending;
      bool m_argValuesChangedPending;
      bool m_defaultValuesChangedPending;
      bool m_topoDirtyPending;
      bool m_dirtyPending;

    private slots:

      void onBindingDirty();

      void onBindingArgInserted(
        unsigned index,
        FTL::CStrRef name,
        FTL::CStrRef typeName
        );
      void onBindingTopoDirty();

      void onBindingArgTypeChanged(
        unsigned index,
        FTL::CStrRef name,
        FTL::CStrRef newTypeName
        );

      void onBindingArgRemoved(
        unsigned index,
        FTL::CStrRef name
        );

      void onBindingArgsReordered(
        FTL::ArrayRef<unsigned> newOrder
        );

      void onBindingArgValueChanged(
        unsigned index,
        FTL::CStrRef name
        );

      void onBindingVarInserted(
        FTL::CStrRef varName,
        FTL::CStrRef varPath,
        FTL::CStrRef typeName,
        FTL::CStrRef extDep
        );

      void onBindingVarRemoved(
        FTL::CStrRef varName,
        FTL::CStrRef varPath
        );
    };

  };

};

#endif // __UI_DFG_DFGController__
