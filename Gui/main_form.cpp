#include "main_form.h"

#include <tests/root.h>
#include <complexities/list_analyzer.h>
#include <msclr\marshal_cppstd.h>

namespace WF = System::Windows::Forms;
namespace Col = System::Collections::Generic;
namespace Sys = System;

std::vector<std::unique_ptr<ds::tests::Test>> createTests()
{
	auto root = std::make_unique<ds::tests::CompositeTest>("root");
    auto mm   = std::make_unique<ds::tests::CompositeTest>("mm");
	auto amt  = std::make_unique<ds::tests::CompositeTest>("amt");
	auto adt  = std::make_unique<ds::tests::CompositeTest>("adt");

	//// TODO 02
	//mm->add_test(std::make_unique<ds::tests::MemoryManagerTest>());
	//mm->add_test(std::make_unique<ds::tests::CompactMemoryManagerTest>());

	//// TODO 03
	//amt->add_test(std::make_unique<ds::tests::ImplicitSequenceTest>());

	//// TODO 04
	//amt->add_test(std::make_unique<ds::tests::ExplicitSequenceTest>());

	//// TODO 05
	//amt->add_test(std::make_unique<ds::tests::ImplicitHierarchyTest>());

	//// TODO 06
	// amt->add_test(std::make_unique<ds::tests::ExplicitHierarchyTest>());
	// amt->add_test(std::make_unique<ds::tests::HierarchyTest>());

	//// TODO 07
	//adt->add_test(std::make_unique<ds::tests::ListTest>());
	//adt->add_test(std::make_unique<ds::tests::ArraysTest>());
	//adt->add_test(std::make_unique<ds::tests::TreeTest>());

	//// TODO 08
	//adt->add_test(std::make_unique<ds::tests::PriorityQueueTest>());

	//// TODO 09
	//adt->add_test(std::make_unique<ds::tests::SequenceTableTest>());

 //   // TODO 11
	//adt->add_test(std::make_unique<ds::tests::NonSequenceTableTest>());

	//// TODO 12
	//adt->add_test(std::make_unique<ds::tests::SortTest>());

	root->add_test(std::move(mm));
	root->add_test(std::move(amt));
	root->add_test(std::move(adt));
	std::vector<std::unique_ptr<ds::tests::Test>> tests;
	tests.emplace_back(std::move(root));
	return tests;
}

std::vector<std::unique_ptr<ds::utils::Analyzer>> createAnalyzers()
{
	std::vector<std::unique_ptr<ds::utils::Analyzer>> analyzers;
	analyzers.emplace_back(std::make_unique<ds::utils::ListsAnalyzer>());
	return analyzers;
}

namespace gui
{
	const int WidthFitHeader = -1;
	const int WidthFitContent = -2;

	template<typename T>
	public value class RawPointer sealed
	{
	public:
		RawPointer(T* ptr) :
			ptr_(ptr)
		{
		}

		property T* Get
		{
			T* get() { return ptr_; }
		}

	private:
		T* ptr_;
	};

	template<typename T>
	RawPointer<T> wrap(T* p)
	{
		return RawPointer<T>(p);
	}

	// Utils:

	Sys::Drawing::Color TestResultToColor(ds::tests::TestResult result)
	{
		switch (result)
		{
		case ds::tests::TestResult::Pass:
			return Sys::Drawing::Color::FromArgb(1, 51, 204, 51);

		case ds::tests::TestResult::Fail:
			return Sys::Drawing::Color::FromArgb(1, 255, 71, 0);

		case ds::tests::TestResult::Partial:
			return Sys::Drawing::Color::FromArgb(1, 255, 255, 102);

		default:
			return Sys::Drawing::Color::White;
		}
	}

	Sys::String^ MakeGroupName(WF::TreeNode^ node)
	{
		Sys::String^ name = node->Text;
		while (node->Parent)
		{
			node = node->Parent;
			name = node->Text + "." + name;
		}
		return name;
	}

	bool IsLeafNode(WF::TreeNode^ node)
	{
		return node->Nodes->Count == 0;
	}

	using TestTreeNodeData = Sys::Tuple<RawPointer<ds::tests::Test>, WF::ListViewGroup^>;
	using AnalyzerTreeNodeData = Sys::Tuple<RawPointer<ds::utils::Analyzer>>;

	ds::tests::Test* GetNodeTest(WF::TreeNode^ node)
	{
		return dynamic_cast<TestTreeNodeData^>(node->Tag)->Item1.Get;
	}

	WF::ListViewGroup^ GetNodeGroup(WF::TreeNode^ node)
	{
		return dynamic_cast<TestTreeNodeData^>(node->Tag)->Item2;
	}

	ds::utils::Analyzer* GetNodeAnalyzer(WF::TreeNode^ node)
	{
		return dynamic_cast<AnalyzerTreeNodeData^>(node->Tag)->Item1.Get;
	}

	Sys::Void SetSubtreeChecked(WF::TreeNode^ node, bool value)
	{
		for each (WF::TreeNode^ child in node->Nodes)
		{
			child->Checked = value;
			SetSubtreeChecked(child, value);
		}
	}

	Sys::Void SetParentChecked(WF::TreeNode^ node)
	{
		WF::TreeNode^ parent = node->Parent;
		while (parent)
		{
			parent->Checked = true;
			parent = parent->Parent;
		}
	}

	Sys::Void SetParentUnchecked(WF::TreeNode^ node)
	{
		WF::TreeNode^ parent = node->Parent;
		if (parent && parent->Checked)
		{
			bool anyChildChecked = false;
			for each (WF::TreeNode ^ child in parent->Nodes)
			{
				anyChildChecked = anyChildChecked || child->Checked;
			}
			if (!anyChildChecked)
			{
				parent->Checked = false;
				SetParentUnchecked(parent);
			}
		}
	}

	/// <summary>
	/// TreeView, v ktorom je vypnuty dvojklik, kvoli problemom s oznacovanim.
	///	https://social.msdn.microsoft.com/Forums/windows/en-US/c1212cfe-170a-47f9-8b5f-e728fa229c59/how-to-prevent-collapse-when-double-click-treenode-?forum=winforms
	/// </summary>
	public ref class NoClickTree
		: public WF::TreeView
	{
	protected:
		Sys::Void WndProc(WF::Message% m) override
		{
			// Suppress WM_LBUTTONDBLCLK
			if (m.Msg == 0x203)
			{
				m.Result = Sys::IntPtr::Zero;
			}
			else
			{
				WF::TreeView::WndProc(m);
			}
			return Sys::Void();
		}
	};

	MainForm::MainForm(
		const std::vector<std::unique_ptr<ds::tests::Test>>& tests,
		const std::vector<std::unique_ptr<ds::utils::Analyzer>>& analyzers
	) :
        treeCheckInProgress_(false)
	{
		InitializeComponent();
		InitializeExecutors();
		RegisterTestOutputIcons();
		RegisterLogMessageIcons();
		RegisterTests(tests);
		RegisterAnalyzers(analyzers);
		SetAnalyzerPath(Sys::Environment::GetFolderPath(Sys::Environment::SpecialFolder::Desktop));
	}

	Sys::Void MainForm::LogInfo(Sys::String^ message)
	{
		LogMessage(LogMessageType::Info, message);
	}

	Sys::Void MainForm::LogWarning(Sys::String^ message)
	{
		LogMessage(LogMessageType::Warning, message);
	}

	Sys::Void MainForm::LogError(Sys::String^ message)
	{
		LogMessage(LogMessageType::Error, message);
	}

	// Deterministic Dispose.
	// Dispose managed data here.
	MainForm::~MainForm()
	{
		// See https://manski.net/2012/01/idisposable-finalizer-and-suppressfinalize/

		if (IsDisposed)
		{
			return;
		}

		if (components_)
		{
			delete components_;
		}

		this->!MainForm();
	}

	// Non-deterministic Finalize.
	// Free unmanaged resources here.
	MainForm::!MainForm()
	{
	}

	Sys::Void MainForm::InitializeComponent()
	{
		//
		// Colors
		//
		auto const BlueColor = Sys::Drawing::Color::FromArgb(54, 78, 111);
		auto const YellowColor = Sys::Drawing::Color::FromArgb(255, 242, 157);
		auto const WhiteColor = Sys::Drawing::Color::FromArgb(255, 255, 255);

		//
		// Fonts
		//
		auto const HeaderFont = gcnew Sys::Drawing::Font(
			L"Microsoft Sans Serif",
			9.75F,
			Sys::Drawing::FontStyle::Bold,
			Sys::Drawing::GraphicsUnit::Point,
			static_cast<Sys::Byte>(238)
		);
		auto const ButtonFont = gcnew Sys::Drawing::Font(
			L"Microsoft Sans Serif",
			8,
			Sys::Drawing::FontStyle::Regular,
			Sys::Drawing::GraphicsUnit::Point,
			static_cast<Sys::Byte>(238)
		);
		auto const TreeViewFont = gcnew Sys::Drawing::Font(
			L"Microsoft Sans Serif",
			10,
			Sys::Drawing::FontStyle::Regular,
			Sys::Drawing::GraphicsUnit::Point,
			static_cast<Sys::Byte>(238)
	    );
		auto const LabelFont = gcnew Sys::Drawing::Font(
			L"Microsoft Sans Serif",
			10,
			Sys::Drawing::FontStyle::Regular,
			Sys::Drawing::GraphicsUnit::Point,
			static_cast<Sys::Byte>(238)
		);

		//
		// Gcnews
		//
		this->splitNorthSouth_ = gcnew WF::SplitContainer();
		this->splitMainArea_ = gcnew WF::SplitContainer();
		this->splitTests_ = gcnew WF::SplitContainer();
		this->panelTestsHeader_ = gcnew WF::Panel();
		this->labelTestsHeader_ = gcnew WF::Label();
		this->listViewTestsLog_ = gcnew WF::ListView();
		this->columnTestType_ = gcnew WF::ColumnHeader();
		this->columnTestMessage_ = gcnew WF::ColumnHeader();
		this->panelTestsResultsHeader_ = gcnew WF::Panel();
		this->buttonTestLogToClipboard_ = gcnew WF::Button();
		this->buttonTestLogToFile_ = gcnew WF::Button();
		this->buttonTestLogClear_ = gcnew WF::Button();
		this->buttonLogToClipboard_ = gcnew WF::Button();
		this->buttonLogToFile_ = gcnew WF::Button();
		this->buttonLogClear_ = gcnew WF::Button();
		this->buttonRunTests_ = gcnew WF::Button();
		this->buttonAnalyzeSelected_ = gcnew WF::Button();
		this->buttonSetAnalyzerPath_ = gcnew WF::Button();
		this->labelTestResultsHeader_ = gcnew WF::Label();
		this->panelAnalysisHeader_ = gcnew WF::Panel();
		this->panelAnalysisOptions_ = gcnew WF::FlowLayoutPanel();
		this->labelAnalysisHeader_ = gcnew WF::Label();
		this->labelReplicationCount_ = gcnew WF::Label();
		this->labelStepSize_= gcnew WF::Label();
		this->labelStepCount_ = gcnew WF::Label();
		this->textBoxReplicationCount_ = gcnew WF::TextBox();
		this->textBoxStepSize_ = gcnew WF::TextBox();
		this->textBoxStepCount_ = gcnew WF::TextBox();
		this->panelTopMenu_ = gcnew WF::Panel();
		this->listViewGlobalLog_ = gcnew WF::ListView();
		this->columnGlobalLogTime_ = gcnew WF::ColumnHeader();
		this->columnGlobalLogType_ = gcnew WF::ColumnHeader();
		this->columnGlobalLogMessage_ = gcnew WF::ColumnHeader();
		this->panelGlobalLogHeader_ = gcnew WF::Panel();
		this->labelLoggerHeader_ = gcnew WF::Label();
		this->treeViewTests_ = gcnew NoClickTree();
		this->treeViewAnalysis_ = gcnew NoClickTree();

		//
		// Suspend layouts
		//
		(cli::safe_cast<Sys::ComponentModel::ISupportInitialize^>(this->splitNorthSouth_))->BeginInit();
		this->splitNorthSouth_->Panel1->SuspendLayout();
		this->splitNorthSouth_->Panel2->SuspendLayout();
		this->splitNorthSouth_->SuspendLayout();
		(cli::safe_cast<Sys::ComponentModel::ISupportInitialize^>(this->splitMainArea_))->BeginInit();
		this->splitMainArea_->Panel1->SuspendLayout();
		this->splitMainArea_->Panel2->SuspendLayout();
		this->splitMainArea_->SuspendLayout();
		(cli::safe_cast<Sys::ComponentModel::ISupportInitialize^>(this->splitTests_))->BeginInit();
		this->splitTests_->Panel1->SuspendLayout();
		this->splitTests_->Panel2->SuspendLayout();
		this->splitTests_->SuspendLayout();
		this->panelTestsHeader_->SuspendLayout();
		this->panelTestsResultsHeader_->SuspendLayout();
		this->panelAnalysisHeader_->SuspendLayout();
		this->panelAnalysisOptions_->SuspendLayout();
		this->panelTopMenu_->SuspendLayout();
		this->panelGlobalLogHeader_->SuspendLayout();
		this->SuspendLayout();

		//
		// splitNorthSouth
		//
		this->splitNorthSouth_->BackColor = BlueColor;
		this->splitNorthSouth_->Dock = WF::DockStyle::Fill;
		this->splitNorthSouth_->FixedPanel = WF::FixedPanel::Panel2;
		this->splitNorthSouth_->Name = L"splitNorthSouth";
		this->splitNorthSouth_->Orientation = WF::Orientation::Horizontal;
		this->splitNorthSouth_->Panel2MinSize = 150;
		this->splitNorthSouth_->Panel1->Padding = WF::Padding(0, 4, 0, 0);
		this->splitNorthSouth_->Panel1->Controls->Add(this->splitMainArea_);
		this->splitNorthSouth_->Panel2->Controls->Add(this->listViewGlobalLog_);
		this->splitNorthSouth_->Panel2->Controls->Add(this->panelGlobalLogHeader_);

		//
		// splitMainArea
		//
		this->splitMainArea_->Dock = WF::DockStyle::Fill;
		this->splitMainArea_->FixedPanel = WF::FixedPanel::Panel2;
		this->splitMainArea_->Margin = WF::Padding(2, 2, 2, 2);
		this->splitMainArea_->Name = L"splitMainArea";
		this->splitMainArea_->Size = Sys::Drawing::Size(1109, 454);
		this->splitMainArea_->SplitterDistance = 700;
		this->splitMainArea_->Panel2MinSize = 300;
		this->splitMainArea_->Panel1->Controls->Add(this->splitTests_);
		this->splitMainArea_->Panel2->Controls->Add(this->treeViewAnalysis_);
		this->splitMainArea_->Panel2->Controls->Add(this->panelAnalysisHeader_);
		this->splitMainArea_->Panel2->Controls->Add(this->panelAnalysisOptions_);

		//
		// splitTests
		//
		this->splitTests_->BackColor = BlueColor;
		this->splitTests_->Dock = WF::DockStyle::Fill;
		this->splitTests_->FixedPanel = WF::FixedPanel::Panel1;
		this->splitTests_->Location = Sys::Drawing::Point(0, 0);
		this->splitTests_->Name = L"splitTests";
		this->splitTests_->Size = Sys::Drawing::Size(475, 454);
		this->splitTests_->SplitterDistance = 400;
		this->splitTests_->TabIndex = 0;
		this->splitTests_->Panel1->Controls->Add(this->treeViewTests_);
		this->splitTests_->Panel1->Controls->Add(this->panelTestsHeader_);
		this->splitTests_->Panel1MinSize = 230;
		this->splitTests_->Panel2->Controls->Add(this->listViewTestsLog_);
		this->splitTests_->Panel2->Controls->Add(this->panelTestsResultsHeader_);

		//
		// panelTestsHeader
		//
		this->panelTestsHeader_->BackColor = YellowColor;
		this->panelTestsHeader_->Controls->Add(this->labelTestsHeader_);
		this->panelTestsHeader_->Controls->Add(this->buttonRunTests_);
		this->panelTestsHeader_->Dock = WF::DockStyle::Top;
		this->panelTestsHeader_->Location = Sys::Drawing::Point(0, 0);
		this->panelTestsHeader_->Name = L"panelTestHeader";
		this->panelTestsHeader_->Size = Sys::Drawing::Size(265, 30);
		this->panelTestsHeader_->TabIndex = 1;

		//
		// labelTestsHeader
		//
		this->labelTestsHeader_->AutoSize = true;
		this->labelTestsHeader_->Font = HeaderFont;
		this->labelTestsHeader_->Location = Sys::Drawing::Point(3, 7);
		this->labelTestsHeader_->Name = L"labelTestsHeader";
		this->labelTestsHeader_->Size = Sys::Drawing::Size(46, 16);
		this->labelTestsHeader_->TabIndex = 0;
		this->labelTestsHeader_->Text = L"Tests";
		this->labelTestsHeader_->TextAlign = Sys::Drawing::ContentAlignment::MiddleLeft;

		//
		// listViewTestLog
		//
		this->listViewTestsLog_->Columns->AddRange(gcnew cli::array<WF::ColumnHeader^>(2) {
			this->columnTestType_,
			this->columnTestMessage_
		});
		this->listViewTestsLog_->BorderStyle = WF::BorderStyle::None;
	    this->listViewTestsLog_->Dock = WF::DockStyle::Fill;
		this->listViewTestsLog_->FullRowSelect = true;
		this->listViewTestsLog_->GridLines = true;
		this->listViewTestsLog_->HideSelection = false;
		this->listViewTestsLog_->Location = Sys::Drawing::Point(0, 30);
		this->listViewTestsLog_->Name = L"listViewTestLog";
		this->listViewTestsLog_->Size = Sys::Drawing::Size(206, 424);
		this->listViewTestsLog_->TabIndex = 1;
		this->listViewTestsLog_->UseCompatibleStateImageBehavior = false;
		this->listViewTestsLog_->View = WF::View::Details;

		//
		// columnTestType
		//
		this->columnTestType_->Text = L"Type";

		//
		// columnTestMessage
		//
		this->columnTestMessage_->Text = L"Message";
		this->columnTestMessage_->Width = 414;

		//
		// panelTestsResultsHeader
		//
		this->panelTestsResultsHeader_->BackColor = YellowColor;
		this->panelTestsResultsHeader_->Controls->Add(this->buttonTestLogToClipboard_);
		this->panelTestsResultsHeader_->Controls->Add(this->buttonTestLogToFile_);
		this->panelTestsResultsHeader_->Controls->Add(this->buttonTestLogClear_);
		this->panelTestsResultsHeader_->Controls->Add(this->labelTestResultsHeader_);
		this->panelTestsResultsHeader_->Dock = WF::DockStyle::Top;
		this->panelTestsResultsHeader_->Location = Sys::Drawing::Point(0, 0);
		this->panelTestsResultsHeader_->Name = L"panelTestResults";
		this->panelTestsResultsHeader_->Size = Sys::Drawing::Size(206, 30);
		this->panelTestsResultsHeader_->TabIndex = 0;

		//
		// buttonTestLogToClipboard
		//
		this->buttonTestLogToClipboard_->Dock = WF::DockStyle::Right;
		this->buttonTestLogToClipboard_->Font = ButtonFont;
		this->buttonTestLogToClipboard_->Location = Sys::Drawing::Point(-44, 0);
		this->buttonTestLogToClipboard_->Name = L"buttonTestLogToClipboard";
		this->buttonTestLogToClipboard_->Size = Sys::Drawing::Size(100, 30);
		this->buttonTestLogToClipboard_->TabIndex = 3;
		this->buttonTestLogToClipboard_->Text = L"Copy to clipboard";
		this->buttonTestLogToClipboard_->UseVisualStyleBackColor = true;
		this->buttonTestLogToClipboard_->Click += gcnew Sys::EventHandler(this, &MainForm::buttonTestLogToClipboard_Click);

		//
		// buttonTestLogToFile
		//
		this->buttonTestLogToFile_->Dock = WF::DockStyle::Right;
		this->buttonTestLogToFile_->Font = ButtonFont;
		this->buttonTestLogToFile_->Location = Sys::Drawing::Point(56, 0);
		this->buttonTestLogToFile_->Name = L"buttonTestLogToFile";
		this->buttonTestLogToFile_->Size = Sys::Drawing::Size(75, 30);
		this->buttonTestLogToFile_->TabIndex = 2;
		this->buttonTestLogToFile_->Text = L"Save to file";
		this->buttonTestLogToFile_->UseVisualStyleBackColor = true;
		this->buttonTestLogToFile_->Click += gcnew Sys::EventHandler(this, &MainForm::buttonTestLogToFile_Click);

		//
		// buttonTestLogClear
		//
		this->buttonTestLogClear_->Dock = WF::DockStyle::Right;
		this->buttonTestLogClear_->Font = ButtonFont;
		this->buttonTestLogClear_->Location = Sys::Drawing::Point(131, 0);
		this->buttonTestLogClear_->Name = L"buttonTestLogClear";
		this->buttonTestLogClear_->Size = Sys::Drawing::Size(75, 30);
		this->buttonTestLogClear_->TabIndex = 1;
		this->buttonTestLogClear_->Text = L"Clear results";
		this->buttonTestLogClear_->UseVisualStyleBackColor = true;
		this->buttonTestLogClear_->Click += gcnew Sys::EventHandler(this, &MainForm::buttonTestLogClear_Click);

		//
		// labelTestResultsHeader
		//
		this->labelTestResultsHeader_->AutoSize = true;
		this->labelTestResultsHeader_->Font = HeaderFont;
		this->labelTestResultsHeader_->Location = Sys::Drawing::Point(4, 7);
		this->labelTestResultsHeader_->Name = L"labelTestResultsHeader";
		this->labelTestResultsHeader_->Size = Sys::Drawing::Size(88, 16);
		this->labelTestResultsHeader_->TabIndex = 0;
		this->labelTestResultsHeader_->Text = L"Test results";
		this->labelTestResultsHeader_->TextAlign = Sys::Drawing::ContentAlignment::MiddleLeft;

		//
		// panelAnalysisHeader
		//
		this->panelAnalysisHeader_->BackColor = YellowColor;
		this->panelAnalysisHeader_->Controls->Add(this->labelAnalysisHeader_);
		this->panelAnalysisHeader_->Controls->Add(this->buttonSetAnalyzerPath_);
		this->panelAnalysisHeader_->Controls->Add(this->buttonAnalyzeSelected_);
		this->panelAnalysisHeader_->Dock = WF::DockStyle::Top;
		this->panelAnalysisHeader_->Location = Sys::Drawing::Point(0, 0);
		this->panelAnalysisHeader_->Margin = WF::Padding(2, 2, 2, 2);
		this->panelAnalysisHeader_->Name = L"panelAnalysisHeader";
		this->panelAnalysisHeader_->Size = Sys::Drawing::Size(300, 30);
		this->panelAnalysisHeader_->TabIndex = 0;

		//
		// labelAnalysisHeader
		//
		this->labelAnalysisHeader_->AutoSize = true;
		this->labelAnalysisHeader_->Font = HeaderFont;
		this->labelAnalysisHeader_->Location = Sys::Drawing::Point(3, 7);
		this->labelAnalysisHeader_->Margin = WF::Padding(2, 0, 2, 0);
		this->labelAnalysisHeader_->Name = L"labelAnalysisHeader";
		this->labelAnalysisHeader_->Size = Sys::Drawing::Size(100, 16);
		this->labelAnalysisHeader_->TabIndex = 0;
		this->labelAnalysisHeader_->Text = L"Analysis";

	    //
		// labelReplicationCount
		//
		this->labelReplicationCount_->Font = LabelFont;
		this->labelReplicationCount_->Margin = WF::Padding(2, 0, 2, 0);
		this->labelReplicationCount_->Name = L"labelReplicationCount";
		this->labelReplicationCount_->Size = Sys::Drawing::Size(130, 16);
		this->labelReplicationCount_->Text = L"Replication count:";

	    //
		// labelStepSize
		//
		this->labelStepSize_->Font = LabelFont;
		this->labelStepSize_->Margin = WF::Padding(2, 0, 2, 0);
		this->labelStepSize_->Name = L"labelStepSize";
		this->labelStepSize_->Size = Sys::Drawing::Size(100, 16);
		this->labelStepSize_->Text = L"Step size:";

	    //
		// labelStepCount
		//
		this->labelStepCount_->Font = LabelFont;
		this->labelStepCount_->Margin = WF::Padding(2, 0, 2, 0);
		this->labelStepCount_->Name = L"labelStepCount";
		this->labelStepCount_->Size = Sys::Drawing::Size(100, 16);
		this->labelStepCount_->Text = L"Step count:";

		//
		// textBoxReplicationCount
		//
		this->textBoxReplicationCount_->Name = L"textBoxReplicationCount";
		this->textBoxReplicationCount_->Size = Sys::Drawing::Size(130, 16);
		this->textBoxReplicationCount_->Text = L"100";

	    //
		// textBoxStepSize
		//
		this->textBoxStepSize_->Name = L"textBoxStepSize";
		this->textBoxStepSize_->Size = Sys::Drawing::Size(100, 16);
		this->textBoxStepSize_->Text = L"10000";

	    //
		// textBoxStepCount
		//
		this->textBoxStepCount_->Name = L"textBoxStepCount";
		this->textBoxStepCount_->Size = Sys::Drawing::Size(100, 16);
		this->textBoxStepCount_->Text = L"10";

		//
		// panelAnalysisOptions
		//
		this->panelAnalysisOptions_->BackColor = YellowColor;
		this->panelAnalysisOptions_->Controls->Add(this->labelReplicationCount_);
		this->panelAnalysisOptions_->Controls->Add(this->labelStepSize_);
		this->panelAnalysisOptions_->Controls->Add(this->labelStepCount_);
		this->panelAnalysisOptions_->SetFlowBreak(this->labelStepCount_, true);
		this->panelAnalysisOptions_->Controls->Add(this->textBoxReplicationCount_);
		this->panelAnalysisOptions_->Controls->Add(this->textBoxStepSize_);
		this->panelAnalysisOptions_->Controls->Add(this->textBoxStepCount_);
		this->panelAnalysisOptions_->Dock = WF::DockStyle::Bottom;
		this->panelAnalysisOptions_->Location = Sys::Drawing::Point(0, 0);
		this->panelAnalysisOptions_->Margin = WF::Padding(2, 2, 2, 2);
		this->panelAnalysisOptions_->Name = L"panelAnalysisOptions";
		this->panelAnalysisOptions_->Size = Sys::Drawing::Size(300, 50);
		this->panelAnalysisOptions_->TabIndex = 0;

		//
		// panelTopMenu
		//
		this->panelTopMenu_->BackColor = BlueColor;
		this->panelTopMenu_->Dock = WF::DockStyle::Top;
		this->panelTopMenu_->Padding = WF::Padding(2);
		this->panelTopMenu_->Location = Sys::Drawing::Point(0, 0);
		this->panelTopMenu_->Name = L"panelTopMenu";
		this->panelTopMenu_->Size = Sys::Drawing::Size(1109, 44);
		this->panelTopMenu_->TabIndex = 0;

		//
		// buttonRunTests
		//
		this->buttonRunTests_->Dock = WF::DockStyle::Right;
		this->buttonRunTests_->Font = ButtonFont;
		this->buttonRunTests_->Name = L"buttonRunTests";
		this->buttonRunTests_->Size = Sys::Drawing::Size(119, 38);
		this->buttonRunTests_->TabIndex = 0;
		this->buttonRunTests_->Text = L"Run selected";
		this->buttonRunTests_->UseVisualStyleBackColor = true;
		this->buttonRunTests_->Click += gcnew Sys::EventHandler(this, &MainForm::buttonRunTests_Click);

	    //
		// buttonAnalyzeSelected
		//
		this->buttonAnalyzeSelected_->Dock = WF::DockStyle::Right;
		this->buttonAnalyzeSelected_->Font = ButtonFont;
		this->buttonAnalyzeSelected_->Name = L"buttonAnalyzeSelected";
		this->buttonAnalyzeSelected_->Size = Sys::Drawing::Size(119, 38);
		this->buttonAnalyzeSelected_->TabIndex = 0;
		this->buttonAnalyzeSelected_->Text = L"Analyze selected";
		this->buttonAnalyzeSelected_->UseVisualStyleBackColor = true;
		this->buttonAnalyzeSelected_->Click += gcnew Sys::EventHandler(this, &MainForm::buttonAnalyzeSelected_Click);

		//
		// buttonAnalyzeSelected
		//
		this->buttonSetAnalyzerPath_->Dock = WF::DockStyle::Right;
		this->buttonSetAnalyzerPath_->Font = ButtonFont;
		this->buttonSetAnalyzerPath_->Name = L"buttonSetaAnalyzerPath";
		this->buttonSetAnalyzerPath_->Size = Sys::Drawing::Size(119, 38);
		this->buttonSetAnalyzerPath_->TabIndex = 0;
		this->buttonSetAnalyzerPath_->Text = L"Choose output path";
		this->buttonSetAnalyzerPath_->UseVisualStyleBackColor = true;
		this->buttonSetAnalyzerPath_->Click += gcnew Sys::EventHandler(this, &MainForm::buttonSetAnalyzerPath_Click);

		//
		// listViewGlobalLog
		//
		this->listViewGlobalLog_->BorderStyle = WF::BorderStyle::None;
		this->listViewGlobalLog_->Columns->AddRange(gcnew cli::array< WF::ColumnHeader^  >(3) {
			this->columnGlobalLogTime_,
				this->columnGlobalLogType_, this->columnGlobalLogMessage_
		});
		this->listViewGlobalLog_->Dock = WF::DockStyle::Fill;
		this->listViewGlobalLog_->FullRowSelect = true;
		this->listViewGlobalLog_->HideSelection = false;
		this->listViewGlobalLog_->Location = Sys::Drawing::Point(0, 30);
		this->listViewGlobalLog_->Name = L"listViewGlobalLog";
		this->listViewGlobalLog_->Size = Sys::Drawing::Size(1109, 113);
		this->listViewGlobalLog_->TabIndex = 1;
		this->listViewGlobalLog_->UseCompatibleStateImageBehavior = false;
		this->listViewGlobalLog_->View = WF::View::Details;

		//
		// columnGlobalLogTime
		//
		this->columnGlobalLogTime_->Text = L"Time";

		//
		// columnGlobalLogType
		//
		this->columnGlobalLogType_->Text = L"Type";

		//
		// columnGlobalLogMessage
		//
		this->columnGlobalLogMessage_->Text = L"Message";
		this->columnGlobalLogMessage_->Width = 909;

		//
		// panelLoggerHeader
		//
		this->panelGlobalLogHeader_->BackColor = YellowColor;
		this->panelGlobalLogHeader_->Controls->Add(this->buttonLogToClipboard_);
		this->panelGlobalLogHeader_->Controls->Add(this->buttonLogToFile_);
		this->panelGlobalLogHeader_->Controls->Add(this->buttonLogClear_);
		this->panelGlobalLogHeader_->Controls->Add(this->labelLoggerHeader_);
		this->panelGlobalLogHeader_->Dock = WF::DockStyle::Top;
		this->panelGlobalLogHeader_->Location = Sys::Drawing::Point(0, 0);
		this->panelGlobalLogHeader_->Name = L"panelLoggerHeader";
		this->panelGlobalLogHeader_->Size = Sys::Drawing::Size(1109, 30);
		this->panelGlobalLogHeader_->TabIndex = 0;

		//
		// buttonLogToClipboard
		//
		this->buttonLogToClipboard_->Dock = WF::DockStyle::Right;
		this->buttonLogToClipboard_->Font = ButtonFont;
		this->buttonLogToClipboard_->Location = Sys::Drawing::Point(859, 0);
		this->buttonLogToClipboard_->Name = L"buttonLogToClipboard";
		this->buttonLogToClipboard_->Size = Sys::Drawing::Size(100, 30);
		this->buttonLogToClipboard_->TabIndex = 3;
		this->buttonLogToClipboard_->Text = L"Copy to clipboard";
		this->buttonLogToClipboard_->UseVisualStyleBackColor = true;
		this->buttonLogToClipboard_->Click += gcnew Sys::EventHandler(this, &MainForm::buttonLogToClipboard_Click);

		//
		// buttonLogToFile
		//
		this->buttonLogToFile_->Dock = WF::DockStyle::Right;
		this->buttonLogToFile_->Font = ButtonFont;
		this->buttonLogToFile_->Location = Sys::Drawing::Point(959, 0);
		this->buttonLogToFile_->Name = L"buttonLogToFile";
		this->buttonLogToFile_->Size = Sys::Drawing::Size(75, 30);
		this->buttonLogToFile_->TabIndex = 2;
		this->buttonLogToFile_->Text = L"Save to file";
		this->buttonLogToFile_->UseVisualStyleBackColor = true;
		this->buttonLogToFile_->Click += gcnew Sys::EventHandler(this, &MainForm::buttonLogToFile_Click);

		//
		// buttonLogClear
		//
		this->buttonLogClear_->Dock = WF::DockStyle::Right;
		this->buttonLogClear_->Font = ButtonFont;
		this->buttonLogClear_->Location = Sys::Drawing::Point(1034, 0);
		this->buttonLogClear_->Name = L"buttonLogClear";
		this->buttonLogClear_->Size = Sys::Drawing::Size(75, 30);
		this->buttonLogClear_->TabIndex = 1;
		this->buttonLogClear_->Text = L"Clear log";
		this->buttonLogClear_->UseVisualStyleBackColor = true;
		this->buttonLogClear_->Click += gcnew Sys::EventHandler(this, &MainForm::buttonLogClear_Click);

		//
		// labelLoggerHeader
		//
		this->labelLoggerHeader_->AutoSize = true;
		this->labelLoggerHeader_->Font = HeaderFont;
		this->labelLoggerHeader_->Location = Sys::Drawing::Point(3, 6);
		this->labelLoggerHeader_->Name = L"labelLoggerHeader";
		this->labelLoggerHeader_->Size = Sys::Drawing::Size(56, 16);
		this->labelLoggerHeader_->TabIndex = 0;
		this->labelLoggerHeader_->Text = L"Logger";
		this->labelLoggerHeader_->TextAlign = Sys::Drawing::ContentAlignment::MiddleLeft;

		//
		// treeViewTests
		//
		this->treeViewTests_->BorderStyle = WF::BorderStyle::None;
		this->treeViewTests_->CheckBoxes = true;
		this->treeViewTests_->Dock = WF::DockStyle::Fill;
		this->treeViewTests_->Font = TreeViewFont;
		this->treeViewTests_->Margin = WF::Padding(4);
		this->treeViewTests_->Name = L"treeViewTests";
		this->treeViewTests_->AfterCheck += gcnew WF::TreeViewEventHandler(this, &MainForm::treeViewTests_AfterCheck);
		this->treeViewTests_->AfterSelect += gcnew WF::TreeViewEventHandler(this, &MainForm::treeViewTests_AfterSelect);

	    //
		// treeViewAnalysis
		//
		this->treeViewAnalysis_->BorderStyle = WF::BorderStyle::None;
		this->treeViewAnalysis_->CheckBoxes = true;
		this->treeViewAnalysis_->Dock = WF::DockStyle::Fill;
		this->treeViewAnalysis_->Font = TreeViewFont;
		this->treeViewAnalysis_->Margin = WF::Padding(4);
		this->treeViewAnalysis_->Name = L"treeViewAnalysis";
		this->treeViewAnalysis_->AfterCheck += gcnew WF::TreeViewEventHandler(this, &MainForm::treeViewAnalysis_AfterCheck);
		this->treeViewAnalysis_->AfterSelect += gcnew WF::TreeViewEventHandler(this, &MainForm::treeViewAnalysis_AfterSelect);

		//
		// MainForm
		//
		this->AutoScaleDimensions = Sys::Drawing::SizeF(6, 13);
		this->AutoScaleMode = WF::AutoScaleMode::Font;
		this->AutoScroll = true;
		this->ClientSize = Sys::Drawing::Size(1200, 800);
		this->Controls->Add(this->splitNorthSouth_);
		this->Name = L"MainForm";
		this->Text = L"Datastructures";
		this->WindowState = WF::FormWindowState::Maximized;
		this->splitNorthSouth_->Panel1->ResumeLayout(false);
		this->splitNorthSouth_->Panel2->ResumeLayout(false);
		(cli::safe_cast<Sys::ComponentModel::ISupportInitialize^>(this->splitNorthSouth_))->EndInit();
		this->splitNorthSouth_->ResumeLayout(false);
		this->splitMainArea_->Panel1->ResumeLayout(false);
		this->splitMainArea_->Panel2->ResumeLayout(false);
		(cli::safe_cast<Sys::ComponentModel::ISupportInitialize^>(this->splitMainArea_))->EndInit();
		this->splitMainArea_->ResumeLayout(false);
		this->splitTests_->Panel1->ResumeLayout(false);
		this->splitTests_->Panel2->ResumeLayout(false);
		(cli::safe_cast<Sys::ComponentModel::ISupportInitialize^>(this->splitTests_))->EndInit();
		this->splitTests_->ResumeLayout(false);
		this->panelTestsHeader_->ResumeLayout(false);
		this->panelTestsHeader_->PerformLayout();
		this->panelTestsResultsHeader_->ResumeLayout(false);
		this->panelTestsResultsHeader_->PerformLayout();
		this->panelAnalysisHeader_->ResumeLayout(false);
		this->panelAnalysisHeader_->PerformLayout();
		this->panelAnalysisOptions_->ResumeLayout(false);
		this->panelAnalysisOptions_->PerformLayout();
		this->panelTopMenu_->ResumeLayout(false);
		this->panelGlobalLogHeader_->ResumeLayout(false);
		this->panelGlobalLogHeader_->PerformLayout();
		this->ResumeLayout(false);

		//
		// Tests controls
		//
		testsControls_.Add(buttonRunTests_);
		testsControls_.Add(treeViewTests_);
		testsControls_.Add(listViewTestsLog_);
		testsControls_.Add(buttonTestLogToClipboard_);
		testsControls_.Add(buttonTestLogToFile_);
		testsControls_.Add(buttonTestLogClear_);

		// Analyzers controls
		analyzersControls_.Add(buttonAnalyzeSelected_);
		analyzersControls_.Add(treeViewAnalysis_);
	}

	Sys::Void MainForm::InitializeExecutors()
	{
		testExecutor_.WorkerSupportsCancellation = false;
		testExecutor_.DoWork += gcnew Sys::ComponentModel::DoWorkEventHandler(this, &MainForm::testExecutor_DoWork);
		testExecutor_.RunWorkerCompleted += gcnew Sys::ComponentModel::RunWorkerCompletedEventHandler(this, &MainForm::testExecutor_RunWorkerCompleted);
		analyzerExecutor_.WorkerSupportsCancellation = false;
		analyzerExecutor_.DoWork += gcnew Sys::ComponentModel::DoWorkEventHandler(this, &MainForm::analyzerExecutor_DoWork);
		analyzerExecutor_.RunWorkerCompleted += gcnew Sys::ComponentModel::RunWorkerCompletedEventHandler(this, &MainForm::analyzerExecutor_RunWorkerCompleted);
	}

	Sys::Void MainForm::RegisterTests(const std::vector<std::unique_ptr<ds::tests::Test>>& tests)
	{
		for (const std::unique_ptr<ds::tests::Test>& test : tests)
		{
			RegisterTest(*test, treeViewTests_->Nodes);
		}
		treeViewTests_->ExpandAll();
	}

	Sys::Void MainForm::RegisterTest(ds::tests::Test& test, WF::TreeNodeCollection^ nodes)
	{
		WF::TreeNode^ node = gcnew WF::TreeNode(gcnew Sys::String(std::string(test.name()).c_str()));
		nodes->Add(node);
		Sys::String^ groupName = MakeGroupName(node);
		WF::ListViewGroup^ group = gcnew WF::ListViewGroup();
		group->Name = groupName;
		group->Header = groupName;
		node->Tag = gcnew TestTreeNodeData(wrap(&test), group);
		listViewTestsLog_->Groups->Add(group);
		if (ds::tests::CompositeTest* compositeTest = dynamic_cast<ds::tests::CompositeTest*>(&test))
		{
			for (const std::unique_ptr<ds::tests::Test>& subTest : compositeTest->subtests())
			{
				RegisterTest(*subTest.get(), node->Nodes);
			}
		}
	}

	Sys::Void MainForm::ResetTests()
	{
		for each (WF::TreeNode^ node in treeViewTests_->Nodes)
		{
			GetNodeTest(node)->reset();
		}
	}

	Sys::Void MainForm::ResetTestLog()
	{
		listViewTestsLog_->Items->Clear();
	}

	Sys::Void MainForm::RunSelectedTests(WF::TreeNodeCollection^ nodes)
	{
		for each (WF::TreeNode^ node in nodes)
		{
			if (IsLeafNode(node) && node->Checked)
			{
				GetNodeTest(node)->run();
			}
			else
			{
				RunSelectedTests(node->Nodes);
			}
		}
	}

	Sys::Void MainForm::RunSelectedAnalyzers(WF::TreeNodeCollection^ nodes)
	{
		for each (WF::TreeNode^ node in nodes)
		{
			if (IsLeafNode(node) && node->Checked)
			{
				GetNodeAnalyzer(node)->analyze();
			}
			else
			{
				RunSelectedAnalyzers(node->Nodes);
			}
		}
	}

	Sys::Void MainForm::ColorizeTestTree(WF::TreeNodeCollection^ nodes)
	{
		for each (WF::TreeNode ^ node in nodes)
		{
			node->BackColor = TestResultToColor(GetNodeTest(node)->result());
			ColorizeTestTree(node->Nodes);
		}
	}

	Sys::Void MainForm::ShowTestsOutput(WF::TreeNodeCollection^ nodes)
	{
		for each (WF::TreeNode ^ node in nodes)
		{
			if (!node->Checked)
			{
				continue;
			}

			if (IsLeafNode(node))
			{
				if (auto* leafTestPtr = dynamic_cast<ds::tests::LeafTest*>(GetNodeTest(node)))
				{
					for (const ds::tests::TestMessage& log : leafTestPtr->output())
					{
						array<Sys::String^>^ columns = gcnew array<Sys::String^>(3)
						{
							gcnew Sys::String(ds::tests::test_message_type_to_string(log.type_).c_str()),
							gcnew Sys::String(log.text_.c_str())
						};
						WF::ListViewItem^ item = gcnew WF::ListViewItem(columns, TestMessageTypeToIndex(log.type_));
						item->Group = GetNodeGroup(node);
						listViewTestsLog_->Items->Add(item);
					}
				}
			}
			else
			{
		        ShowTestsOutput(node->Nodes);
			}
		}
	}

	Sys::Void MainForm::ShowAnalyzersOutput(WF::TreeNodeCollection^ nodes)
	{
		for each (WF::TreeNode^ node in nodes)
		{
			if (!node->Checked)
			{
				continue;
			}

			if (IsLeafNode(node))
			{
				if (auto* leafAnalyzer = dynamic_cast<ds::utils::LeafAnalyzer*>(GetNodeAnalyzer(node)))
				{
					if (leafAnalyzer->wasSuccessful())
					{
						LogInfo(gcnew Sys::String(("Output of " + leafAnalyzer->getName() + " was written to " + leafAnalyzer->getOutputPath().string()).c_str()));
					}
					else
					{
						LogError(gcnew Sys::String(("Analyzer " + leafAnalyzer->getName() + " failed.").c_str()));
					}
				}
			}
			else
			{
				ShowAnalyzersOutput(node->Nodes);
			}
		}
	}

	Sys::Void MainForm::BeforeRunSelectedTests()
	{
		listViewTestsLog_->BeginUpdate();
		treeViewTests_->BeginUpdate();
		ResetTests();
		ResetTestLog();
		for each (WF::Control^ control in testsControls_)
		{
			control->Enabled = false;
		}
	}

	Sys::Void MainForm::AfterRunSelectedTests()
	{
		for each (WF::Control^ control in testsControls_)
		{
			control->Enabled = true;
		}
		ColorizeTestTree(treeViewTests_->Nodes);
		ShowTestsOutput(treeViewTests_->Nodes);
		AdjustTestsOutputColumns();
		treeViewTests_->EndUpdate();
		listViewTestsLog_->EndUpdate();
	}

	Sys::Void MainForm::BeforeRunSelectedAnalyzers()
	{
		for each (WF::Control^ control in analyzersControls_)
		{
			control->Enabled = false;
		}
	}

	Sys::Void MainForm::AfterRunSelectedAnalyzers()
	{
		for each (WF::Control^ control in analyzersControls_)
		{
			control->Enabled = true;
		}
		ShowAnalyzersOutput(treeViewAnalysis_->Nodes);
	}

	Sys::Void MainForm::AdjustTestsOutputColumns()
	{
		bool anyOutput = listViewTestsLog_->Items->Count != 0;
		columnTestType_->Width = anyOutput ? WidthFitContent : WidthFitContent;
		columnTestMessage_->Width = anyOutput ? WidthFitContent : WidthFitContent;
	}

	Sys::Void MainForm::RegisterAnalyzers(const std::vector<std::unique_ptr<ds::utils::Analyzer>>& analyzers)
	{
		for (const std::unique_ptr<ds::utils::Analyzer>& analyzer : analyzers)
		{
			RegisterAnalyzer(*analyzer, treeViewAnalysis_->Nodes);
		}
		treeViewAnalysis_->ExpandAll();
	}

	Sys::Void MainForm::RegisterAnalyzer(ds::utils::Analyzer& analyzer, WF::TreeNodeCollection^ nodes)
	{
		WF::TreeNode^ node = gcnew WF::TreeNode(gcnew Sys::String(analyzer.getName().c_str()));
		node->Tag = gcnew AnalyzerTreeNodeData(wrap(&analyzer));
		nodes->Add(node);
		if (auto* compositeAnalyzer = dynamic_cast<ds::utils::CompositeAnalyzer*>(&analyzer))
		{
			for (auto const& subAnalyzer : compositeAnalyzer->getAnalyzers())
			{
				RegisterAnalyzer(*subAnalyzer, node->Nodes);
			}
		}
	}

	Sys::Int32 testOutputTypeToInt(ds::tests::TestMessageType t)
	{
		return static_cast<Sys::Int32>(static_cast<std::underlying_type_t<ds::tests::TestMessageType>>(t));
	}

	Sys::Int32 logMessageTypeToInt(MainForm::LogMessageType t)
	{
		return static_cast<Sys::Int32>(static_cast<std::underlying_type_t<MainForm::LogMessageType>>(t));
	}

	Sys::Void MainForm::RegisterTestOutputIcons()
	{
		testOutputTypeToIndex_.Add(testOutputTypeToInt(ds::tests::TestMessageType::Fail), 0);
		testOutputTypeToIndex_.Add(testOutputTypeToInt(ds::tests::TestMessageType::Pass), 1);
		testOutputTypeToIndex_.Add(testOutputTypeToInt(ds::tests::TestMessageType::Info), 2);
		listViewTestsLog_->SmallImageList = gcnew WF::ImageList();
		listViewTestsLog_->SmallImageList->Images->Add(gcnew Sys::Drawing::Bitmap("images/fail.png"));
		listViewTestsLog_->SmallImageList->Images->Add(gcnew Sys::Drawing::Bitmap("images/pass.png"));
		listViewTestsLog_->SmallImageList->Images->Add(gcnew Sys::Drawing::Bitmap("images/info.png"));
	}

	Sys::Void MainForm::RegisterLogMessageIcons()
	{
		logMessageTypeToIndex_.Add(logMessageTypeToInt(MainForm::LogMessageType::Info), 0);
		logMessageTypeToIndex_.Add(logMessageTypeToInt(MainForm::LogMessageType::Warning), 1);
		logMessageTypeToIndex_.Add(logMessageTypeToInt(MainForm::LogMessageType::Error), 2);
		listViewGlobalLog_->SmallImageList = gcnew WF::ImageList();
		listViewGlobalLog_->SmallImageList->Images->Add(gcnew Sys::Drawing::Bitmap("images/info.png"));
		listViewGlobalLog_->SmallImageList->Images->Add(gcnew Sys::Drawing::Bitmap("images/warning.png"));
		listViewGlobalLog_->SmallImageList->Images->Add(gcnew Sys::Drawing::Bitmap("images/fail.png"));
	}

	Sys::Int32 MainForm::TestMessageTypeToIndex(ds::tests::TestMessageType type)
	{
		Sys::Int32 i;
		return testOutputTypeToIndex_.TryGetValue(testOutputTypeToInt(type), i) ? i : -1;
	}

	Sys::Int32 MainForm::LogMessageTypeToIndex(LogMessageType type)
	{
		Sys::Int32 i;
		return logMessageTypeToIndex_.TryGetValue(logMessageTypeToInt(type), i) ? i : -1;
	}

	Sys::String^ MainForm::GlobalLogToString()
	{
		Sys::Text::StringBuilder^ sb = gcnew Sys::Text::StringBuilder();
		for each (WF::ListViewItem^ item in listViewGlobalLog_->Items)
		{
			Sys::String^ time = item->SubItems[0]->Text;
			Sys::String^ type = item->SubItems[1]->Text;
			Sys::String^ message = item->SubItems[2]->Text;
			sb->AppendFormat("[{0}] {1,-8} {2}{3}", time, type, message, Sys::Environment::NewLine);
		}
		return sb->ToString();
	}

	Sys::String^ MainForm::TestOutputToString()
	{
		System::Text::StringBuilder^ sb = gcnew System::Text::StringBuilder();
		WF::ListViewGroup^ currentGroup = nullptr;

		for each (WF::ListViewItem ^ item in listViewTestsLog_->Items)
		{
			if (item->Group && item->Group != currentGroup)
			{
				sb->AppendLine();
				sb->AppendFormat("{0}{1}", item->Group->Header, System::Environment::NewLine);
				currentGroup = item->Group;
			}

			Sys::String^ type = item->SubItems[0]->Text;
			Sys::String^ message = item->SubItems[1]->Text;
			sb->AppendFormat("\t{0,-7} {1}{2}", type, message, System::Environment::NewLine);
		}

		return sb->ToString();
	}

	Sys::Void MainForm::TrySetAnalysisParameters()
	{
		Sys::UInt32 replCount;
		if (Sys::UInt32::TryParse(this->textBoxReplicationCount_->Text, replCount))
		{
			for each (WF::TreeNode ^ node in this->treeViewAnalysis_->Nodes)
			{
				GetNodeAnalyzer(node)->setReplicationCount(replCount);
			}
		}
		else
		{
			this->LogError("Failed to set replication count. Using the previous value.");
		}

	    Sys::UInt32 stepSize;
		if (Sys::UInt32::TryParse(this->textBoxStepSize_->Text, stepSize))
		{
			for each (WF::TreeNode ^ node in this->treeViewAnalysis_->Nodes)
			{
				GetNodeAnalyzer(node)->setStepSize(stepSize);
			}
		}
		else
		{
			this->LogError("Failed to set step size. Using the previous value.");
		}

	    Sys::UInt32 stepCount;
		if (Sys::UInt32::TryParse(this->textBoxStepCount_->Text, stepCount))
		{
			for each (WF::TreeNode ^ node in this->treeViewAnalysis_->Nodes)
			{
				GetNodeAnalyzer(node)->setStepCount(stepCount);
			}
		}
		else
		{
			this->LogError("Failed to set step count. Using the previous value.");
		}
	}

	Sys::Void MainForm::SetAnalyzerPath(Sys::String^ str)
	{
		for each (WF::TreeNode ^ node in treeViewAnalysis_->Nodes)
		{
			GetNodeAnalyzer(node)->setOutputDirectory(msclr::interop::marshal_as<std::string>(str));
		}
		LogInfo("Analyzer output directory was set to: " + str);
	}

	Sys::Void MainForm::SaveToFile(Sys::String^ str)
	{
		try
		{
			Sys::IO::Stream^ fileStream;
			WF::SaveFileDialog^ dialog = gcnew WF::SaveFileDialog();
			dialog->Filter = "txt files (*.txt)|*.txt|All files (*.*)|*.*";
			dialog->FilterIndex = 1;
			dialog->RestoreDirectory = true;
			if (dialog->ShowDialog() == WF::DialogResult::OK)
			{
				if ((fileStream = dialog->OpenFile()) != nullptr)
				{
					Sys::IO::StreamWriter^ sw = gcnew Sys::IO::StreamWriter(fileStream);
					sw->Write(str);
					sw->Flush();
					sw->Close();
					fileStream->Close();
					LogInfo("Log was saved to file.");
				}
				else
				{
					LogError("Failed to open the file.");
				}
			}
		}
		catch (Sys::Exception^)
		{
			LogError("Failed to save the log to the file.");
		}
	}

	Sys::Void MainForm::SaveToClipboard(Sys::String^ str)
	{
		try
		{
			WF::Clipboard::SetText(str);
			LogInfo("Log was copied to the clipboard.");
		}
		catch (System::Exception^)
		{
			LogError("Failed to copy log to the clipboard.");
		}
	}

	Sys::Void MainForm::LogMessage(LogMessageType type, Sys::String^ message)
	{
		Sys::String^ timestamp = System::DateTime::Now.ToString("HH:mm:ss");
		Sys::String^ typeStr = "Info";
		array<Sys::String^>^ columns = gcnew array<Sys::String^>(3)
		{
			timestamp,
				typeStr,
				message
		};
		WF::ListViewItem^ item = gcnew WF::ListViewItem(columns, LogMessageTypeToIndex(type));
		listViewGlobalLog_->Items->Insert(0, item);
		columnGlobalLogTime_->Width = WidthFitContent;
		columnGlobalLogMessage_->Width = WidthFitContent;
	}

	Sys::Void MainForm::buttonRunTests_Click(Sys::Object^ sender, Sys::EventArgs^ e)
	{
		testExecutor_.RunWorkerAsync();
	}

	Sys::Void MainForm::buttonAnalyzeSelected_Click(Sys::Object^ sender, Sys::EventArgs^ e)
	{
		analyzerExecutor_.RunWorkerAsync();
	}

	Sys::Void MainForm::treeViewTests_AfterSelect(Sys::Object^ sender, WF::TreeViewEventArgs^ e)
	{
		treeViewTests_->SelectedNode = nullptr;
		e->Node->Checked = !e->Node->Checked;
	}

	Sys::Void MainForm::treeViewTests_AfterCheck(Sys::Object^ sender, WF::TreeViewEventArgs^ e)
	{
		if (!treeCheckInProgress_)
		{
			treeCheckInProgress_ = true;
			SetSubtreeChecked(e->Node, e->Node->Checked);
			if (e->Node->Checked)
			{
				SetParentChecked(e->Node);
			}
			else
			{
				SetParentUnchecked(e->Node);
			}
			treeCheckInProgress_ = false;
		}
	}

	Sys::Void MainForm::treeViewAnalysis_AfterSelect(Sys::Object^ sender, WF::TreeViewEventArgs^ e)
	{
		treeViewAnalysis_->SelectedNode = nullptr;
		e->Node->Checked = !e->Node->Checked;
	}

	Sys::Void MainForm::treeViewAnalysis_AfterCheck(Sys::Object^ sender, WF::TreeViewEventArgs^ e)
	{
		if (!treeCheckInProgress_)
		{
			treeCheckInProgress_ = true;
			SetSubtreeChecked(e->Node, e->Node->Checked);
			if (e->Node->Checked)
			{
				SetParentChecked(e->Node);
			}
			else
			{
				SetParentUnchecked(e->Node);
			}
			treeCheckInProgress_ = false;
		}
	}

	Sys::Void MainForm::buttonLogClear_Click(Sys::Object^ sender, Sys::EventArgs^ e)
	{
		listViewGlobalLog_->Items->Clear();
	}

	Sys::Void MainForm::buttonLogToFile_Click(Sys::Object^ sender, Sys::EventArgs^ e)
	{
		if (listViewGlobalLog_->Items->Count > 0)
		{
			SaveToFile(GlobalLogToString());
		}
	}

	Sys::Void MainForm::buttonLogToClipboard_Click(Sys::Object^ sender, Sys::EventArgs^ e)
	{
		if (listViewGlobalLog_->Items->Count > 0)
		{
			SaveToClipboard(GlobalLogToString());
		}
	}

	Sys::Void MainForm::buttonSetAnalyzerPath_Click(Sys::Object^ sender, Sys::EventArgs^ e)
	{
		try
		{
			WF::FolderBrowserDialog dialog;
			if (dialog.ShowDialog() == WF::DialogResult::OK)
			{
			    SetAnalyzerPath(dialog.SelectedPath);
			}
		}
		catch (Sys::Exception^)
		{
			LogError("Failed to pick output directory.");
		}
	}

	Sys::Void MainForm::buttonTestLogClear_Click(Sys::Object^ sender, Sys::EventArgs^ e)
	{
		ResetTestLog();
		ResetTests();
		ColorizeTestTree(treeViewTests_->Nodes);
		for each (WF::TreeNode ^ node in treeViewTests_->Nodes)
		{
			SetSubtreeChecked(node, false);
		}
	}

	Sys::Void MainForm::buttonTestLogToFile_Click(Sys::Object^ sender, Sys::EventArgs^ e)
	{
		if (listViewTestsLog_->Items->Count > 0)
		{
			SaveToFile(TestOutputToString());
		}
	}

	Sys::Void MainForm::buttonTestLogToClipboard_Click(Sys::Object^ sender, Sys::EventArgs^ e)
	{
		if (listViewTestsLog_->Items->Count > 0)
		{
			SaveToClipboard(TestOutputToString());
		}
	}

	Sys::Void MainForm::testExecutor_DoWork(Sys::Object^ sender, Sys::ComponentModel::DoWorkEventArgs^ e)
	{
		this->Invoke(gcnew WF::MethodInvoker(this, &MainForm::BeforeRunSelectedTests));
		RunSelectedTests(treeViewTests_->Nodes);
	}

	Sys::Void MainForm::testExecutor_RunWorkerCompleted(Sys::Object^ sender, Sys::ComponentModel::RunWorkerCompletedEventArgs^ e)
	{
		this->Invoke(gcnew WF::MethodInvoker(this, &MainForm::AfterRunSelectedTests));
	}

	public delegate void LogInvoker(Sys::String^);

	Sys::Void MainForm::analyzerExecutor_DoWork(Sys::Object^ sender, Sys::ComponentModel::DoWorkEventArgs^ e)
	{
		this->Invoke(gcnew WF::MethodInvoker(this, &MainForm::BeforeRunSelectedAnalyzers));
		try
		{
			this->TrySetAnalysisParameters();
			this->RunSelectedAnalyzers(treeViewAnalysis_->Nodes);
		}
		catch (const std::exception& e)
		{
			this->Invoke(gcnew LogInvoker(this, &MainForm::LogError), gcnew Sys::String((std::string("Exception: ") + e.what()).c_str()));
		}
	}

	Sys::Void MainForm::analyzerExecutor_RunWorkerCompleted(Sys::Object^ sender, Sys::ComponentModel::RunWorkerCompletedEventArgs^ e)
	{
		this->Invoke(gcnew WF::MethodInvoker(this, &MainForm::AfterRunSelectedAnalyzers));
	}
}

[Sys::STAThread]
void main(array<Sys::String^>^ args)
{
	initHeapMonitor();

    auto const tests = createTests();
	auto const analyzers = createAnalyzers();

	WF::Application::EnableVisualStyles();
	WF::Application::SetCompatibleTextRenderingDefault(false);
	gui::MainForm mainForm(tests, analyzers);
	WF::Application::Run(% mainForm);
}