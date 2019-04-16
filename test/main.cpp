#include <html/html_doc.hpp>
#include <iostream>

using namespace std;
using namespace html;

int main()
{
    html_doc doc = html_doc::parse(R"foo(
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Frameset//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
<title>Srun3000计费管理系统-IPV4在线用户</title>
<style media="all" type="text/css">
	@import "style/all.css";body {
	margin-top: 0px;
}
</style>
<script language="javascript" src="script/ajax.js"></script>
<script language="javascript">
function setVal(obj)
{
	if(obj.checked)
	{
		document.form2.sel_val.value += obj.value + ",";
	}
	else
	{
		document.form2.sel_val.value = document.form2.sel_val.value.replace(obj.value+",", "");
	}
}

function drop(ip)
{
	if(!confirm("是否要下线连接？"))
		return;
	if(ip == "all")
	{
		if(document.form2.sel_val.value == "")
		{
			alert("请选择要下线的连接");
			return;
		}
		var d = "action=drops&user_ip="+document.form2.sel_val.value;
		var res = postData("online_user_ipv4.php", "post", d);
		alert(res);
	}
	else
	{
		var d = "action=drop&user_ip="+ip;
		var res = postData("online_user_ipv4.php", "post", d);
		if(res == "ok")
		{
			alert("下线成功");
		}
		else
		{
			alert("操作失败");
		}
	}
	document.location.reload();
}

function select_all(obj)
{
	var i=0;
	for(i=0; i<=Number(document.form2.chk_sum.value); i++)
	{
		try{
			if(document.getElementById("cb"+i).checked != obj.checked)
				document.getElementById("cb"+i).click();
			}catch(E)
			{
				break;
			}
	}
}
</script>
</head>
<body>
<table class="maintab" align="center">
	<tr>
		<td width="10">
		</td>
		<td>	
			<table cellpadding="0" cellspacing="0"  width="98%" align="" height="30">
				<form name="form2" action="online_user_ipv4.php" method="get">
					<input type="hidden" name="action" value="">
					<input type="hidden" name="sel_val">
        </form>
			
			</table>
			<table cellpadding="0" cellspacing="0"  width="98%" align="" height="30">
				<tr align="center" bgcolor="#D1DCEB">		
					<td width="20" height="21" class="maintd">
			     	<input type="checkbox" onclick="select_all(this)">
			    </td>
					<td class="maintd">IP</td>
					<td class="maintd">登录时间</td>
					<td class="maintd">入流量</td>
					<td class="maintd">出流量</td>
					<td class="maintd">NAS IP</td>
					<td class="maintd">VLAN ID</td>
					<td class="maintd">MAC</td>
					<td class="maintd">余额</td>
					<td class="maintd">下行速率</td>
					<td class="maintd">上行速率</td>
					<td class="maintd">终端</td>					
					<td class="maintd">可用流量</td>
					<td class="maintd">可用时长</td>
					
				</tr>
								<tr align="center">
					<td width="20" height="21" class="maintd">
			     	<input type="checkbox" id="cb0" value="11873002" onclick="setVal(this)">
			    </td>
					<td class="maintd">59.66.124.59</td>
					<td class="maintd">2019-04-16 12:46:14</td>
					<td class="maintd">22.46M</td>
					<td class="maintd">11.12M</td>
					<td class="maintd">166.111.204.121</td>
					<td class="maintd">0</td>
					<td class="maintd">3429.8f90.4d8a</td>
					<td class="maintd">19.0700</td>
					<td class="maintd">0Mbps</td>
					<td class="maintd">0Mbps</td>
					<td class="maintd">iPhone</td>					
					<td class="maintd">0B</td>
					<td class="maintd">0</td>
					
				</tr>
							</table>
			<table cellpadding="0" cellspacing="0" width="98%">
		  	<tr> 
		     <td height="28">
		     	选中项：<input type="button" value="下线" onclick="drop('all')">
		     </td>
		    </tr>
		 </table>
		</td>
	</tr>
        <input type="hidden" name="chk_sum" value="0">
</table>
</body>
</html>
)foo");
    cout << doc << endl;
}
