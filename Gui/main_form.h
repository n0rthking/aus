#pragma once

#include <tests/_details/test.hpp>
#include <complexities/complexity_analyzer.h>
#include <vector>
#include <memory>

namespace gui
{
	namespace Col = System::Collections::Generic;
	namespace Sys = System;
	namespace WF = System::Windows::Forms;

	/// <summary>
	/// Main window of the application.
	/// </summary>
	public ref class MainForm : public WF::Form
	{
	public:
		/// <summary>
		/// Creates main window of the program containing given tests.
		/// </summary>
		/// <param name="tests"></param>
		MainForm(
			const std::vector<std::unique_ptr<ds::tests::Test>>& tests,
			const std::vector<std::unique_ptr<ds::utils::Analyzer>>& analyzers
		);

		Sys::Void LogInfo(Sys::String^ message);
		Sys::Void LogWarning(Sys::String^ message);
		Sys::Void LogError(Sys::String^ message);

	protected:
		/// <summary>
		/// Dispose. Cleans managed resources.
		/// </summary>
		~MainForm();

		/// <summary>
		/// Finalize. Cleans unmanaged resources.
		/// </summary>
		!MainForm();

	public:
		enum class LogMessageType
		{
			Info,
			Warning,
			Error
		};

	private:
		Sys::Void AdjustTestsOutputColumns();
		Sys::Void AfterRunSelectedTests();
		Sys::Void AfterRunSelectedAnalyzers();
		Sys::Void BeforeRunSelectedAnalyzers();
		Sys::Void BeforeRunSelectedTests();
		Sys::Void ColorizeTestTree(WF::TreeNodeCollection^ nodes);
		Sys::String^ GlobalLogToString();
		Sys::Void InitializeComponent();
		Sys::Void InitializeExecutors();
		Sys::Void LogMessage(LogMessageType type, Sys::String^ message);
		Sys::Int32 LogMessageTypeToIndex(LogMessageType type);
		Sys::Void RegisterAnalyzers(const std::vector<std::unique_ptr<ds::utils::Analyzer>>& analyzers);
		Sys::Void RegisterAnalyzer(ds::utils::Analyzer& analyzer, WF::TreeNodeCollection^ nodes);
		Sys::Void RegisterLogMessageIcons();
        Sys::Void RegisterTests(const std::vector<std::unique_ptr<ds::tests::Test>>& tests);
        Sys::Void RegisterTest(ds::tests::Test& test, WF::TreeNodeCollection^ nodes);
		Sys::Void RegisterTestOutputIcons();
		Sys::Void ResetTests();
		Sys::Void ResetTestLog();
		Sys::Void RunSelectedTests(WF::TreeNodeCollection^ nodes);
		Sys::Void RunSelectedAnalyzers(WF::TreeNodeCollection^ nodes);
		Sys::Void SaveToClipboard(Sys::String^ str);
		Sys::Void SaveToFile(Sys::String^ str);
		Sys::Void ShowTestsOutput(WF::TreeNodeCollection^ nodes);
		Sys::Void ShowAnalyzersOutput(WF::TreeNodeCollection^ nodes);
		Sys::Int32 TestMessageTypeToIndex(ds::tests::TestMessageType type);
		Sys::String^ TestOutputToString();
		Sys::Void TrySetAnalysisParameters();
		Sys::Void SetAnalyzerPath(Sys::String^ str);

	    Sys::Void buttonRunTests_Click(Sys::Object^ sender, Sys::EventArgs^ e);
		Sys::Void buttonAnalyzeSelected_Click(Sys::Object^ sender, Sys::EventArgs^ e);
		Sys::Void treeViewTests_AfterSelect(Sys::Object^ sender, WF::TreeViewEventArgs^ e);
		Sys::Void treeViewTests_AfterCheck(Sys::Object^ sender, WF::TreeViewEventArgs^ e);
		Sys::Void treeViewAnalysis_AfterSelect(Sys::Object^ sender, WF::TreeViewEventArgs^ e);
		Sys::Void treeViewAnalysis_AfterCheck(Sys::Object^ sender, WF::TreeViewEventArgs^ e);
		Sys::Void buttonLogClear_Click(Sys::Object^ sender, Sys::EventArgs^ e);
		Sys::Void buttonLogToFile_Click(Sys::Object^ sender, Sys::EventArgs^ e);
		Sys::Void buttonLogToClipboard_Click(Sys::Object^ sender, Sys::EventArgs^ e);
		Sys::Void buttonSetAnalyzerPath_Click(Sys::Object^ sender, Sys::EventArgs^ e);
		Sys::Void buttonTestLogClear_Click(Sys::Object^ sender, Sys::EventArgs^ e);
		Sys::Void buttonTestLogToFile_Click(Sys::Object^ sender, Sys::EventArgs^ e);
		Sys::Void buttonTestLogToClipboard_Click(Sys::Object^ sender, Sys::EventArgs^ e);
		Sys::Void testExecutor_DoWork(Sys::Object^ sender, Sys::ComponentModel::DoWorkEventArgs^ e);
		Sys::Void testExecutor_RunWorkerCompleted(Sys::Object^ sender, Sys::ComponentModel::RunWorkerCompletedEventArgs^ e);
		Sys::Void analyzerExecutor_DoWork(Sys::Object^ sender, Sys::ComponentModel::DoWorkEventArgs^ e);
		Sys::Void analyzerExecutor_RunWorkerCompleted(Sys::Object^ sender, Sys::ComponentModel::RunWorkerCompletedEventArgs^ e);

	private:
		Sys::ComponentModel::Container^ components_;
		WF::SplitContainer^ splitNorthSouth_;
		WF::SplitContainer^ splitMainArea_;
		WF::SplitContainer^ splitTests_;
		WF::Panel^ panelTopMenu_;
		WF::Panel^ panelTestsHeader_;
		WF::Panel^ panelTestsResultsHeader_;
		WF::Panel^ panelAnalysisHeader_;
		WF::FlowLayoutPanel^ panelAnalysisOptions_;
		WF::Panel^ panelGlobalLogHeader_;
		WF::Button^ buttonRunTests_;
		WF::Button^ buttonAnalyzeSelected_;
		WF::Button^ buttonSetAnalyzerPath_;
		WF::Button^ buttonLogToClipboard_;
		WF::Button^ buttonLogToFile_;
		WF::Button^ buttonLogClear_;
		WF::Button^ buttonTestLogToClipboard_;
		WF::Button^ buttonTestLogToFile_;
		WF::Button^ buttonTestLogClear_;
		WF::Label^ labelTestsHeader_;
		WF::Label^ labelTestResultsHeader_;
		WF::Label^ labelLoggerHeader_;
		WF::Label^ labelAnalysisHeader_;
		WF::Label^ labelReplicationCount_;
		WF::Label^ labelStepSize_;
		WF::Label^ labelStepCount_;
		WF::TextBox^ textBoxReplicationCount_;
		WF::TextBox^ textBoxStepSize_;
		WF::TextBox^ textBoxStepCount_;
		WF::ListView^ listViewTestsLog_;
		WF::ListView^ listViewGlobalLog_;
		WF::ColumnHeader^ columnTestMessage_;
		WF::ColumnHeader^ columnGlobalLogMessage_;
		WF::ColumnHeader^ columnGlobalLogTime_;
		WF::ColumnHeader^ columnGlobalLogType_;
		WF::ColumnHeader^ columnTestType_;
		WF::TreeView^ treeViewTests_;
		WF::TreeView^ treeViewAnalysis_;
		WF::ImageList^ testLogIconList_;
		WF::ImageList^ globalLogIconList_;
		Col::List<WF::Control^> testsControls_;
		Col::List<WF::Control^> analyzersControls_;
		Sys::ComponentModel::BackgroundWorker testExecutor_;
		Sys::ComponentModel::BackgroundWorker analyzerExecutor_;
		Col::Dictionary<Sys::Int32, Sys::Int32> testOutputTypeToIndex_;
		Col::Dictionary<Sys::Int32, Sys::Int32> logMessageTypeToIndex_;
		bool treeCheckInProgress_;
	};
}
