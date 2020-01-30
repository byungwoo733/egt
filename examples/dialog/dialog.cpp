/*
 * Copyright (C) 2018 Microchip Technology Inc.  All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <egt/ui>
#include <experimental/filesystem>
#include <string>

using namespace std;
using namespace egt;
namespace fs = std::experimental::filesystem;

int main(int argc, const char** argv)
{
    Application app(argc, argv, "dialog");

    TopWindow win;

    auto dialog_size = Application::instance().screen()->size() * 0.75;

    auto layout = make_shared<VerticalBoxSizer>();
    win.add(expand(layout));

    auto grid = make_shared<StaticGrid>(std::make_tuple(3, 1));
    grid->height(50);
    grid->color(Palette::ColorId::bg, Palette::gray);
    layout->add(expand_horizontal(grid));

    auto logo = make_shared<ImageLabel>(Image("icon:128px/egt_logo_black.png"));
    grid->add(center(logo), 1, 0);

    auto vsizer = make_shared<VerticalBoxSizer>();
    layout->add(expand(vsizer));

    auto dialog_result = std::make_shared<Label>();

    auto hsizer = make_shared<FlexBoxSizer>(Justification::none);
    vsizer->add(expand(hsizer));

    std::string root_dir = fs::current_path().string();

    auto fileopen_dialog = std::make_shared<FileOpenDialog>(root_dir, dialog_size);
    fileopen_dialog->on_selected([fileopen_dialog, dialog_result]()
    {
        dialog_result->text("File OpenDialog: " + fileopen_dialog->selected() + " Selected");
        fileopen_dialog->hide();
    });
    win.add(fileopen_dialog);

    auto fileopen_dialog_btn = std::make_shared<Button>("File Open");
    fileopen_dialog_btn->margin(5);
    hsizer->add(fileopen_dialog_btn);
    fileopen_dialog_btn->on_click([fileopen_dialog, dialog_result](Event&)
    {
        dialog_result->text("");
        fileopen_dialog->show_modal(true);
    });

    auto filesave_dialog = std::make_shared<FileSaveDialog>(root_dir, dialog_size);
    win.add(filesave_dialog);
    filesave_dialog->on_selected([filesave_dialog, dialog_result]()
    {
        dialog_result->text("File SaveDialog: " + filesave_dialog->selected() + " Selected");
        filesave_dialog->hide();
    });

    auto filesave_dialog_btn = std::make_shared<Button>("File Save");
    filesave_dialog_btn->margin(5);
    hsizer->add(filesave_dialog_btn);
    filesave_dialog_btn->on_click([filesave_dialog, dialog_result](Event&)
    {
        dialog_result->text("");
        filesave_dialog->show_modal(true);
    });

    auto message_dialog = std::make_shared<Dialog>(dialog_size);
    message_dialog->title("Message Dialog Example");
    auto text = std::make_shared<TextBox>("This is an Example of the Ensemble Graphics "
                                          "Toolkit Dialog with two buttons");
    text->readonly(true);
    message_dialog->widget(expand(text));
    message_dialog->button(Dialog::ButtonId::button1, "OK");
    message_dialog->button(Dialog::ButtonId::button2, "Cancel");
    win.add(message_dialog);

    message_dialog->on_button1_click([message_dialog, dialog_result]()
    {
        dialog_result->text("Message Dialog: OK button clicked");
    });

    message_dialog->on_button2_click([message_dialog, dialog_result]()
    {
        dialog_result->text("Message Dialog: Cancel button clicked");
    });

    auto message_dialog_btn = std::make_shared<Button>("Message Dialog");
    message_dialog_btn->margin(5);
    hsizer->add(message_dialog_btn);
    message_dialog_btn->on_click([message_dialog, dialog_result](Event&)
    {
        dialog_result->text("");
        message_dialog->show_modal(true);
    });

    auto list_dialog = std::make_shared<Dialog>(dialog_size);
    list_dialog->title("List Dialog Box Example");
    list_dialog->button(Dialog::ButtonId::button1, "OK");
    list_dialog->button(Dialog::ButtonId::button2, "Cancel");
    win.add(list_dialog);

    auto dlist0 = std::make_shared<ListBox>();
    for (auto x = 0; x < 25; x++)
        dlist0->add_item(std::make_shared<StringItem>("item " + std::to_string(x), Rect(), AlignFlag::left | AlignFlag::center));
    list_dialog->widget(expand(dlist0));

    list_dialog->on_button1_click([list_dialog, dialog_result, dlist0]()
    {
        auto select = dynamic_cast<StringItem*>(dlist0->item_at(dlist0->selected()).get())->text();
        dialog_result->text("List Dialog: " + select + " Selected");
    });

    list_dialog->on_button2_click([list_dialog, dialog_result, dlist0]()
    {
        dialog_result->text("List Dialog: Cancel button clicked");
    });

    auto list_dialog_btn = std::make_shared<Button>("List Dialog");
    list_dialog_btn->margin(5);
    hsizer->add(list_dialog_btn);
    list_dialog_btn->on_click([list_dialog, dialog_result](Event&)
    {
        dialog_result->text("");
        list_dialog->show_modal(true);
    });

    auto slider_dialog = std::make_shared<Dialog>(dialog_size);
    slider_dialog->title("Slider Dialog Example");
    slider_dialog->button(Dialog::ButtonId::button1, "OK");
    slider_dialog->button(Dialog::ButtonId::button2, "Cancel");
    win.add(slider_dialog);

    auto slider1 = std::make_shared<Slider>();
    slider1->slider_flags().set({Slider::SliderFlag::round_handle, Slider::SliderFlag::show_label});
    slider1->value(50);
    slider_dialog->widget(expand(slider1));

    slider_dialog->on_button1_click([slider_dialog, dialog_result, slider1]()
    {
        auto select = slider1->value();
        dialog_result->text("Slider Dialog: value = " + std::to_string(select));
    });

    slider_dialog->on_button2_click([slider_dialog, dialog_result, slider1]()
    {
        dialog_result->text("Slider Dialog: Cancel button clicked");
    });

    auto slider_dialog_btn = std::make_shared<Button>("Slider Dialog");
    slider_dialog_btn->margin(5);
    hsizer->add(slider_dialog_btn);
    slider_dialog_btn->on_click([slider_dialog, dialog_result](Event&)
    {
        dialog_result->text("");
        slider_dialog->show_modal(true);
    });

    vsizer->add(center(dialog_result));

    win.show();

    return app.run();
}
