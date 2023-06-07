<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:output method="html" indent="yes" doctype-public="-//W3C//DTD HTML 4.0 Transitional//EN"/>
	<xsl:template match="morph">
		<h1>Компания (<xsl:value-of select="id/text()"/>)</h1>
		<p><b>Наименование</b>&#160; <xsl:value-of select="name/text()"/></p>
		<p><b>E-mail</b>&#160; <xsl:value-of select="email/text()"/></p>
		<p><b>ИНН</b>&#160; <xsl:value-of select="inn/text()"/></p>
		<p><b>Юридический адрес</b>&#160; <xsl:value-of select="uaddr/text()"/></p>
		<p><b>Почтовый адрес</b>&#160; <xsl:value-of select="paddr/text()"/></p>
	</xsl:template>
</xsl:stylesheet>