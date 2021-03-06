Option Strict Off
Option Explicit Off

Imports System
Imports EnvDTE
Imports EnvDTE80
Imports EnvDTE90
Imports EnvDTE90a
Imports EnvDTE100
Imports System.Diagnostics
Imports Microsoft.VisualBasic
Imports Microsoft.VisualBasic.ControlChars

Public Module 黑火
    ' 如果要确定某个标识符是否为有效的 C 标识符，
    '   则此例程有多种用法。
    '   这些标识符不包括限定语法，例如:
    '   foo.bar 无效
    '   foo 有效
    ' 参数:    用于测试有效 C 标识符的字符串。
    ' 返回值:  True: 传递的参数是有效的 C 标识符。
    '               False: 传递的参数是无效的 C 标识符。
    Function ValidId(ByVal Id As String)
        ValidId = True

        ' 不允许空字符串
        '  (如何用“有”来标识“无”?)。
        If Id = "" Then
            ValidId = False
            Exit Function
        End If

        For i = 1 To Len(Id)
            If Mid(Id, i, 1) < "a" Or Mid(Id, i, 1) > "z" Then
                If Mid(Id, i, 1) < "A" Or Mid(Id, i, 1) > "Z" Then
                    If Mid(Id, i, 1) < "0" Or Mid(Id, i, 1) > "9" Then
                        If Mid(Id, i, 1) <> "_" Then
                            ValidId = False
                        End If
                    End If
                End If
            End If
        Next

        If Mid(Id, 1, 1) >= "0" And Mid(Id, 1, 1) <= "9" Then
            ValidId = False
        End If

    End Function

    '把形如: e:\project\src\tool\main.cpp 简化为 src\tool\main.cpp
    Function ShortenPath(ByVal path As String)
        Dim pos As Integer
        Dim slashcnt As Integer

        ShortenPath = path

        ' 不允许空字符串
        If path = "" Then
            Exit Function
        End If

        pos = InStrRev(path, "\")
        Do While slashcnt < 2
            'ext = Mid(ext, pos, (Len(ext) - pos + 1))
            pos = InStrRev(path, "\", pos - 1)
            slashcnt += 1
        Loop

        ShortenPath = Mid(path, pos + 1, Len(path) - pos)
    End Function

    ' 给文件加上文件头，注明版权、作者、文档创建日期等信息
    ' 请把authorName改为自己的名字
    Sub DocumentFileHeader()
        Dim doc As Document
        Dim docName As String
        Dim companyName As String = "黑火"
        Dim authorName As String = "洪坤安"
        Dim copyrightText As String = String.Format("Copyright (c) {0} {1}. All rights reserved.", Date.Now.Year, companyName)
        Dim text As String

        ' 从程序中获得文件的名字
        doc = DTE.ActiveDocument

        '获得当前编辑类的名字
        docName = doc.Path + doc.Name
        docName = ShortenPath(docName)

        ' 将添加焦点定位在文件首部
        doc.Selection.StartOfDocument()

        text = ""
        text += "///<------------------------------------------------------------------------------" + vbCrLf
        text += "//< @file:   " + docName + vbCrLf
        text += "//< @author: " + authorName + vbCrLf
        text += "//< @date:   " + String.Format("{0:D}", Date.Now) + " " + String.Format("{0}:{1}:{2}", DateTime.Now.Hour, DateTime.Now.Minute, DateTime.Now.Second) + vbCrLf 'System.DateTime.Now.ToLongDateString 'String.Format("{0:D}", Date.Now) 
        text += "//< @brief:  " + vbCrLf
        text += "//< " + copyrightText + vbCrLf
        text += "///<------------------------------------------------------------------------------" + vbCrLf
        text += vbCrLf

        doc.Selection.Text = text
    End Sub


    ' 确保当前的头文件可以只被包含一次。
    ' 说明: 向当前头文件中添加代码，使其只能在每个 c/cpp 文件中包含一次。
    Sub OneTimeInclude()
        Dim ext As String
        Dim DocName As String
        Dim examp As String
        Dim selection As EnvDTE.TextSelection
        Dim pos As Integer
        Dim ControlVarName As String

        ext = DTE.ActiveDocument.Name
        If ext = "" Then
            If MsgBox("The file you are working with does not have a file extension." + Lf + "Are you sure this is a C/C++ header file?", 4) = MsgBoxResult.Cancel Then
                Exit Sub
            End If
            ext = "nofilenamegiven.h"
        End If
        DocName = LCase(ext)
        pos = InStr(ext, ".")
        Do While pos <> 1
            ext = Mid(ext, pos, (Len(ext) - pos + 1))
            pos = InStr(ext, ".")
        Loop
        ext = LCase(ext)
        pos = InStr(DocName, ".")

        If ext = ".h" Or ext = ".hpp" Then
            DTE.ActiveDocument.Selection.StartOfDocument(False)
            examp = "_" + Left(DocName, pos - 1) + "_" + LCase(Right(ext, Len(ext) - 1)) + "_"
            ControlVarName = examp
            selection = DTE.ActiveDocument.Selection
            If ValidId(examp) = True Then
                selection.Text = "#ifndef " + ControlVarName + CStr(Lf) + "#define " + ControlVarName + CStr(Lf)
                DTE.ActiveDocument.Selection.EndOfDocument(False)
                selection.Text = CStr(Lf) + "#endif //" + ControlVarName
            Else
                MsgBox(ControlVarName + " is not a valid c identifier." + Lf + "please re-run the macro with a valid C identifier")
            End If
        Else
            MsgBox("This macro can only be run on .h or .hpp files")
        End If
    End Sub

    ' 根据文件类别给文件加上文件头
    ' 如果是h头文件：添加#ifndef语句，及文件头注释
    ' 如果是cpp文件：文件头注释
    Sub 生成文件头()
        Dim ext As String
        Dim DocName As String
        Dim pos As Integer

        ' 获取文件类型后缀
        ext = DTE.ActiveDocument.Name
        pos = InStr(ext, ".")
        Do While pos <> 1
            ext = Mid(ext, pos, (Len(ext) - pos + 1))
            pos = InStr(ext, ".")
        Loop
        ext = LCase(ext)

        ' 根据是h文件还是cpp文件进行文件头生成
        If ext = ".h" Or ext = ".hpp" Then
            OneTimeInclude()
            DocumentFileHeader()
        Else
            DocumentFileHeader()
        End If
    End Sub

    Sub 去除行尾的空格符()
        'Public Sub RemoveLineEndBlanks()

        'DESCRIPTION: 去除行尾的空格符。
        '
        Dim lineIdx As Integer = 1
        Dim objSel As TextSelection = ActiveDocument.Selection
        Dim objActive As VirtualPoint

        DTE.UndoContext.Open("RemoveLineEndBlanks")

        Try
            Do While True

                objSel.GotoLine(lineIdx, False)
                objSel.EndOfLine()

                objSel.DeleteWhitespace()

                objActive = objSel.ActivePoint
                If objActive.AtEndOfDocument Then
                    Exit Do
                End If

                lineIdx = lineIdx + 1
            Loop
        Finally
            'If an error occured, then need to make sure that the undo context is cleaned up.
            'Otherwise, the editor can be left in a perpetual undo context
            DTE.UndoContext.Close()
        End Try
    End Sub

    Sub AttachToProc(ByVal targetproc As String)
        Dim attached As Boolean = False
        Dim proc As EnvDTE.Process
        Dim pos As Integer

        For Each proc In DTE.Debugger.LocalProcesses
            pos = InStr(proc.Name, targetproc)

            If (pos <> 0) Then
                proc.Attach()
                attached = True
                'Exit For
            End If
        Next

        If attached = False Then
            MsgBox(targetproc + " is not running")
        End If

    End Sub

    Sub 附加到pkserver()
        AttachToProc("pkserver.exe")
    End Sub

    Sub 附加到gameserver()
        AttachToProc("gameserver.exe")
    End Sub

    Sub 附加到gateserver()
        AttachToProc("gateserver.exe")
    End Sub

    Sub 附加到gmserver()
        AttachToProc("gmserver.exe")
    End Sub
End Module
