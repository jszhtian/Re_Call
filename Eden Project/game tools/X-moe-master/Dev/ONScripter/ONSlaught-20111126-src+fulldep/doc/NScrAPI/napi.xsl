<xsl:transform version="1.0"
xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="html" encoding="UTF-8"/>

<xsl:key name="char1" match="command" use="substring(@name,1,1)"/>


<xsl:template match="/api">

<xsl:variable name="root" select="/"/>
<xsl:for-each select="$root">
<html>
<head><title><xsl:value-of select="api/title[1]" /></title><link rel="stylesheet" type="text/css" href="napi.css" /></head>
<body>
<xsl:call-template name="api-header">
  <xsl:with-param name="root" select="$root" />
</xsl:call-template>

<div id="QUICKTABLE">
<div id="UNFRAMED"><div id="LIST">
<xsl:call-template name="api-categories">
  <xsl:with-param name="root" select="$root" />
</xsl:call-template>
</div></div>

<div id="SIDEBAR">
<xsl:for-each select="api/functions/command">
<xsl:sort select="@name"/>
<xsl:element name="a"><xsl:attribute name="href">#<xsl:if test="boolean(@id)"><xsl:value-of select="@id"/></xsl:if><xsl:if test="not(boolean(@id))"><xsl:value-of select="@name"/></xsl:if></xsl:attribute>
<xsl:value-of select="@name"/></xsl:element><br />
</xsl:for-each>
</div>

</div>
<div id="MAIN">
<xsl:for-each select="api/functions/command">
<xsl:sort select="@name"/>
<xsl:call-template name="api-command">
  <xsl:with-param name="root" select="$root" />
</xsl:call-template>
</xsl:for-each>
</div>
<div id="FOOTER"></div>
</body>
</html>
</xsl:for-each>

</xsl:template>


<xsl:template match="/api-page[@name='frameset']">

<html>
<head><title><xsl:value-of select="document(api-src/@href)/api/title" /></title><link rel="stylesheet" type="text/css" href="napi.css" /></head>
<frameset cols="180,70%">
  <frameset rows="150,80%">
    <xsl:element name="frame">
      <xsl:attribute name="name">index</xsl:attribute>
      <xsl:attribute name="src"><xsl:value-of select="api-index/@href" /></xsl:attribute>
    </xsl:element>
    <xsl:element name="frame">
      <xsl:attribute name="name">side</xsl:attribute>
      <xsl:attribute name="src"><xsl:value-of select="api-side/@href" /></xsl:attribute>
    </xsl:element>
  </frameset>
  <xsl:element name="frame">
    <xsl:attribute name="name">main</xsl:attribute>
    <xsl:attribute name="src"><xsl:value-of select="api-main/@href" /></xsl:attribute>
  </xsl:element>
</frameset>
</html>

</xsl:template>


<xsl:template match="/api-page[@name='main']">

<xsl:variable name="root" select="document(api-src/@href)"/>
<xsl:for-each select="$root">
<html>
<head><title><xsl:value-of select="api/title[1]" /></title><link rel="stylesheet" type="text/css" href="napi.css" /></head>
<body>
<xsl:call-template name="api-header">
  <xsl:with-param name="root" select="$root" />
</xsl:call-template>

<div id="QUICKTABLE">
<div id="LIST">
<xsl:call-template name="api-categories">
  <xsl:with-param name="root" select="$root" />
</xsl:call-template>
</div>
</div>
<div id="MAIN">
<xsl:for-each select="api/functions/command">
<xsl:sort select="@name"/>
<xsl:call-template name="api-command">
  <xsl:with-param name="root" select="$root" />
</xsl:call-template>
</xsl:for-each>
</div>
<div id="FOOTER"></div>
</body>
</html>
</xsl:for-each>

</xsl:template>


<xsl:template match="/api-page[@name='index']">

<xsl:variable name="root" select="document(api-src/@href)"/>
<xsl:variable name="type" select="@type"/>
<xsl:variable name="main" select="api-main/@href"/>
<xsl:variable name="ind" select="api-index/@href"/>
<xsl:variable name="side" select="api-side/@href"/>
<xsl:for-each select="$root">
<html>
<head><link rel="stylesheet" type="text/css" href="napi.css" /></head>
<body>

<xsl:if test="boolean($type='func')">
<div id="INDEX">

[ <xsl:element name="a"><xsl:attribute name="href"><xsl:value-of select="$ind"/></xsl:attribute>Alphabet</xsl:element> ]

<h5>Categories</h5>
<xsl:for-each select="api/categories/category">

<xsl:variable name="categoryid" select="@id"/>
<xsl:if test="boolean($root/api/functions/command[@category=$categoryid])">
<div class="Category">
<xsl:element name="a"><xsl:attribute name="href"><xsl:value-of select="$main"/>#category_<xsl:value-of select="$categoryid"/></xsl:attribute>
<xsl:attribute name="target">main</xsl:attribute><xsl:value-of select="."/></xsl:element>
</div>
</xsl:if>

</xsl:for-each>
</div>
</xsl:if>

<xsl:if test="boolean($type='alpha')">
<div id="INDEX">

[ <xsl:element name="a"><xsl:attribute name="href"><xsl:value-of select="$ind"/></xsl:attribute>Categories</xsl:element> ]

<h5>Alphabet</h5>

  [<xsl:element name="a"><xsl:attribute name="href"><xsl:value-of select="$side"/>##</xsl:attribute><xsl:attribute name="target">side</xsl:attribute>#</xsl:element>]

<xsl:variable name="alphabet" select="'|abcdefghijklmnopqrstuvwxyz'"/>
<xsl:for-each select="api/functions/command[generate-id(.)=generate-id(key('char1', substring(@name,1,1))[1])]">
<xsl:sort select="@name"/>
<xsl:variable name="Init" select="substring(@name,1,1)"/>
<xsl:variable name="isalpha" select="string-length(substring-before($alphabet,$Init))"/>
<xsl:if test="$isalpha &gt; 0">
  <xsl:if test="not(preceding-sibling::command[substring(@name,1,1)=$Init])">
  [<xsl:element name="a"><xsl:attribute name="href"><xsl:value-of select="$side"/>#<xsl:value-of select="$Init"/></xsl:attribute>
  <xsl:attribute name="target">side</xsl:attribute><xsl:value-of select="$Init"/></xsl:element>]
  </xsl:if>
</xsl:if>

</xsl:for-each>

</div>
</xsl:if>

</body>
</html>
</xsl:for-each>

</xsl:template>


<xsl:template match="/api-page[@name='sidebar']">

<xsl:variable name="root" select="document(api-src/@href)"/>
<xsl:variable name="main" select="api-main/@href"/>
<xsl:for-each select="$root">
<html>
<head><link rel="stylesheet" type="text/css" href="napi.css" /></head>
<body>
<div>

<a id="#" />

<xsl:variable name="alphabet" select="'|abcdefghijklmnopqrstuvwxyz'"/>
<xsl:for-each select="api/functions/command[generate-id(.)=generate-id(key('char1', substring(@name,1,1))[1])]">
<xsl:sort select="@name"/>
<xsl:variable name="Init" select="substring(@name,1,1)"/>
<xsl:variable name="isalpha" select="string-length(substring-before($alphabet,$Init))"/>
<xsl:if test="$isalpha &gt; 0">
  <xsl:element name="a"><xsl:attribute name="id"><xsl:value-of select="$Init"/></xsl:attribute></xsl:element>
</xsl:if>

<xsl:for-each select="key('char1', substring(@name,1,1))">
<xsl:sort select="@name"/>

<xsl:element name="a"><xsl:attribute name="href"><xsl:value-of select="$main"/>#<xsl:if test="boolean(@id)"><xsl:value-of select="@id"/></xsl:if><xsl:if test="not(boolean(@id))"><xsl:value-of select="@name"/></xsl:if></xsl:attribute>
<xsl:attribute name="target">main</xsl:attribute>
<xsl:value-of select="@name"/></xsl:element><br />

</xsl:for-each>
</xsl:for-each>
</div>
</body>
</html>
</xsl:for-each>

</xsl:template>


<xsl:template match="/api-page[@name='havenot']">

<xsl:variable name="have-eng" select="@have"/>
<xsl:variable name="havenot-eng" select="@havenot"/>
<xsl:variable name="root" select="document(api-src/@href)"/>
<xsl:for-each select="$root">
<html>
<head><title><xsl:value-of select="api/scr-engines/engine[@id=$have-eng]/@label" /> Commands Not Supported By <xsl:value-of select="api/scr-engines/engine[@id=$havenot-eng]/@label" /></title><link rel="stylesheet" type="text/css" href="napi.css" /></head>
<body>

<div id="QUICKTABLE">
<div id="LIST">
<xsl:call-template name="api-categories-havenot">
  <xsl:with-param name="root" select="$root" />
  <xsl:with-param name="have-eng" select="$have-eng" />
  <xsl:with-param name="havenot-eng" select="$havenot-eng" />
</xsl:call-template>
</div>
</div>
<div id="MAIN">
<xsl:for-each select="api/functions/command">
<xsl:sort select="@name"/>
<xsl:if test="boolean(scr/.=$have-eng)">
<xsl:if test="not(boolean(scr/.=$havenot-eng))">
<xsl:call-template name="api-command">
  <xsl:with-param name="root" select="$root" />
</xsl:call-template>
</xsl:if></xsl:if>
</xsl:for-each>
</div>
<div id="FOOTER"></div>
</body>
</html>
</xsl:for-each>

</xsl:template>


<xsl:template name="api-header">
<xsl:param name="root" />

<div id="HEADER">
<a name="top" id="top"></a>
<h5>Background Color Explanations</h5>
<table id="BGCOLOR_EXPLANATION_TABLE">
<xsl:for-each select="api/cmd-types/cmd-type">
<tr><td width="75"><xsl:element name="div"><xsl:attribute name="class"><xsl:value-of select="@id"/></xsl:attribute><xsl:value-of select="@title"/></xsl:element></td><td><xsl:value-of select="."/></td></tr>
</xsl:for-each>
</table>
<h5>Argument Type Explanations</h5>
<table id="VALUE_EXPLANATION_TABLE"><col span="1" width="75" bgcolor="#E0E0E0" /><col span="1" />
<xsl:for-each select="api/arg-types/arg-type">
<tr><td><xsl:value-of select="@type"/></td><td><xsl:value-of select="." /></td></tr>
</xsl:for-each>
</table>
<h5>Allowed Numeric Ranges</h5>
<table id="NUMBER_RANGE_TABLE"><col span="1" width="100" bgcolor="#E0E0E0" /><col span="1" width="75" bgcolor="#E0E0E0" /><col span="1" />
<xsl:for-each select="api/numbers/number">
<tr><td><xsl:value-of select="@type"/> Number</td><td><xsl:value-of select="@min"/> - <xsl:value-of select="@max"/></td><td><xsl:value-of select="."/></td></tr>
</xsl:for-each>
</table>
<h5>Related Game Engines</h5>
<table id="VALUE_EXPLANATION_TABLE"><col span="1" width="75" bgcolor="#E0E0E0" /><col span="1" />
<xsl:for-each select="api/scr-engines/engine">
<tr><td>
<xsl:if test="boolean(@href)">
<xsl:element name="a"><xsl:attribute name="target">_blank</xsl:attribute><xsl:attribute name="href"><xsl:value-of select="@href"/></xsl:attribute><xsl:value-of select="@label"/></xsl:element></xsl:if>
<xsl:if test="not(boolean(@href))"><xsl:value-of select="@label"/></xsl:if>
</td><td><xsl:value-of select="." />
</td></tr>
</xsl:for-each>
</table><br />
</div>

</xsl:template>


<xsl:template name="api-categories">
<xsl:param name="root" />

<xsl:for-each select="api/categories/category">
<xsl:variable name="categoryid" select="@id"/>
<xsl:if test="boolean($root/api/functions/command[@category=$categoryid])">
<xsl:element name="a"><xsl:attribute name="id">category_<xsl:value-of select="$categoryid"/></xsl:attribute></xsl:element>
<h5><xsl:value-of select="."/></h5>

<xsl:for-each select="$root/api/functions/command[@category=$categoryid]">
<xsl:call-template name="api-category-item">
  <xsl:with-param name="root" select="$root" />
</xsl:call-template>
</xsl:for-each>
</xsl:if>
</xsl:for-each>

</xsl:template>


<xsl:template name="api-categories-havenot">
<xsl:param name="root" />
<xsl:param name="have-eng" />
<xsl:param name="havenot-eng" />

<xsl:for-each select="api/categories/category">
<xsl:variable name="categoryid" select="@id"/>
<xsl:if test="boolean($root/api/functions/command[@category=$categoryid])">
<xsl:element name="a"><xsl:attribute name="id">category_<xsl:value-of select="$categoryid"/></xsl:attribute></xsl:element>
<h5><xsl:value-of select="."/></h5>

<xsl:for-each select="$root/api/functions/command[@category=$categoryid]">
<xsl:if test="boolean(scr/.=$have-eng)">
<xsl:if test="not(boolean(scr/.=$havenot-eng))">
<xsl:call-template name="api-category-item">
  <xsl:with-param name="root" select="$root" />
</xsl:call-template>
</xsl:if></xsl:if>
</xsl:for-each>
</xsl:if>
</xsl:for-each>

</xsl:template>


<xsl:template name="api-category-item">
<xsl:param name="root" />

<xsl:variable name="formatdesc" select="format[1]/format-string"/>
<xsl:variable name="typeid" select="@type"/>
<xsl:variable name="typelabel" select="/api/cmd-types/cmd-type[@id=$typeid]/@type"/>
<xsl:element name="div"><xsl:attribute name="class"><xsl:text>Word </xsl:text><xsl:value-of select="$typeid"/></xsl:attribute>
<span class="WordLiteral">
<xsl:element name="a">
<xsl:attribute name="title">Ver.<xsl:value-of select="@version"/><xsl:text> </xsl:text><xsl:value-of select="$typelabel"/>:<xsl:text> &#xA;</xsl:text><xsl:if test="boolean($formatdesc/@desc)"><xsl:value-of select="$formatdesc/@desc"/></xsl:if><xsl:if test="not(boolean($formatdesc/@desc))"><xsl:value-of select="$formatdesc"/></xsl:if></xsl:attribute>
<xsl:attribute name="href">#<xsl:if test="boolean(@id)"><xsl:value-of select="@id"/></xsl:if><xsl:if test="not(boolean(@id))"><xsl:value-of select="@name"/></xsl:if></xsl:attribute><xsl:value-of select="@name"/></xsl:element></span><span class="Function">
<xsl:for-each select="simple-desc/node()">
<xsl:variable name="dnode" select="."/>
<xsl:choose>
<xsl:when test="name($dnode)='b'"><b><xsl:value-of select="$dnode"/></b></xsl:when>
<xsl:otherwise><xsl:value-of select="$dnode"/></xsl:otherwise>
</xsl:choose>
</xsl:for-each></span><div class="Space" /></xsl:element>

</xsl:template>


<xsl:template name="api-command">
<xsl:param name="root" />

<xsl:variable name="categoryid" select="@category"/>
<xsl:variable name="typeid" select="@type"/>
<xsl:variable name="typelabel" select="$root/api/cmd-types/cmd-type[@id=$typeid]/@type"/>
<xsl:element name="a"><xsl:attribute name="id">
<xsl:if test="boolean(@id)"><xsl:value-of select="@id"/></xsl:if>
<xsl:if test="not(boolean(@id))"><xsl:value-of select="@name"/></xsl:if>
</xsl:attribute></xsl:element>
<h2><span class="WordVersion">Ver.<xsl:value-of select="@version"/></span><span class="WordField">[<xsl:value-of select="$typelabel"/>]</span>
<xsl:variable name="scrid1" select="scr[position() = 1]"/>
<span class="WordField">( <xsl:for-each select="scr"><xsl:variable name="scrid" select="."/>
<xsl:if test="$scrid != $scrid1">, </xsl:if><xsl:value-of select="$root/api/scr-engines/engine[@id=$scrid]/@short"/></xsl:for-each> )</span>
<br /><xsl:value-of select="@name"/></h2>
<h4><a class="WordCategory"><xsl:attribute name="href">#category_<xsl:value-of select="$categoryid"/></xsl:attribute><xsl:value-of select="$root/api/categories/category[@id=$categoryid]"/></a></h4>
  <xsl:for-each select="format">
<h3><xsl:value-of select="format-string"/></h3>
<div class="Arguments">
    <xsl:for-each select="arg">
<span class="ArgType"><xsl:value-of select="@type"/></span><span class="ArgMeaning"><xsl:value-of select="."/></span><div class="Space" />
    </xsl:for-each>
</div>
  </xsl:for-each>
<div class="ContentBody">
<p class="Description"><xsl:for-each select="description/node()">
<xsl:variable name="dnode" select="."/>
<xsl:choose>
<xsl:when test="name($dnode)='br'"><br /></xsl:when>
<xsl:when test="name($dnode)='b'"><b><xsl:value-of select="$dnode"/></b></xsl:when>
<xsl:when test="name($dnode)='indent'"><span class="Indent"/></xsl:when>
<xsl:otherwise><xsl:value-of select="$dnode" disable-output-escaping="yes"/></xsl:otherwise>
</xsl:choose>
</xsl:for-each></p>
<xsl:if test="boolean(notice)">
<xsl:for-each select="notice">
<div class="Notice">
<div class="NoticeHead"><xsl:text disable-output-escaping="yes">&#x203b;</xsl:text></div>
<div class="NoticeBody"><xsl:for-each select="node()">
<xsl:variable name="dnode" select="."/>
<xsl:choose>
<xsl:when test="name($dnode)='br'"><br /></xsl:when>
<xsl:when test="name($dnode)='b'"><b><xsl:value-of select="$dnode"/></b></xsl:when>
<xsl:otherwise><xsl:value-of select="$dnode"/></xsl:otherwise>
</xsl:choose>
</xsl:for-each></div>
</div>
</xsl:for-each></xsl:if>
<xsl:for-each select="example">
<div class="Example">
<xsl:variable name="type" select="@type"/>
<span class="ExHeading"><xsl:value-of select="$root/api/scr-engines/engine[@id=$type]/@label"/> Example:</span><br />
<div class="ExComment"><xsl:value-of select="comment" disable-output-escaping="yes"/></div>
<pre class="ExSource"><xsl:value-of select="code"/></pre>
</div>
</xsl:for-each>
<hr />
<xsl:for-each select="related">
<xsl:variable name="refname" select="@cmd"/>
<xsl:variable name="refcmd" select="$root/api/functions/command[@name = $refname]"/>
<xsl:variable name="refcmdid" select="$root/api/functions/command[@id = $refname]"/>
<xsl:if test="boolean($refcmd)">
<xsl:element name="a"><xsl:attribute name="href">#<xsl:if test="boolean($refcmd/@id)"><xsl:value-of select="$refcmd/@id"/></xsl:if><xsl:if test="not(boolean($refcmd/@id))"><xsl:value-of select="$refname"/></xsl:if></xsl:attribute>
<xsl:value-of select="$refname"/></xsl:element>
</xsl:if>
<xsl:if test="not(boolean($refcmd))">
<xsl:if test="boolean($refcmdid)">
<xsl:element name="a"><xsl:attribute name="href">#<xsl:value-of select="$refcmdid/@id"/></xsl:attribute><xsl:value-of select="$refcmdid/@name"/></xsl:element>
</xsl:if>
<xsl:if test="not(boolean($refcmdid))"><xsl:value-of select="$refname"/></xsl:if>
</xsl:if>
 / </xsl:for-each>
<xsl:if test="boolean(related)"><hr /></xsl:if>
<a href="#top">page top</a> / <a href="#LIST">list</a> / <a href="#MAIN">main</a><br />
</div>

</xsl:template>


</xsl:transform>
