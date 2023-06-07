<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:output method="html" indent="yes" doctype-public="-//W3C//DTD HTML 4.0 Transitional//EN"/>
	<xsl:template match="morph">
		<h1>Валюта (<xsl:value-of select="id/text()"/>)</h1>
		<p><b>Наименование</b>&#160; <xsl:value-of select="name/text()"/></p>
		<p><b>Код</b>&#160; <xsl:value-of select="code/text()"/></p>
	</xsl:template>
</xsl:stylesheet>