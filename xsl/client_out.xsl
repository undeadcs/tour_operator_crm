<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:output method="html" indent="yes" doctype-public="-//W3C//DTD HTML 4.0 Transitional//EN"/>
	<xsl:template match="morph">
		<h1>Клиент физ.лицо (<xsl:value-of select="id/text()"/>)</h1>
		<p><b>ФИО</b>&#160; <xsl:value-of select="fio/text()"/></p>
		<p><b>ФИО латиницей</b>&#160; <xsl:value-of select="fiolatin/text()"/></p>
		<p><b>Дата рождения</b>&#160; <xsl:value-of select="substring(born,0,10)"/></p>
		<p><b>Пол</b>&#160; <xsl:choose>
			<xsl:when test="sex/text()='1'">Мужской</xsl:when>
			<xsl:when test="sex/text()='2'">Женский</xsl:when>
			<xsl:otherwise>мутант</xsl:otherwise>
		</xsl:choose></p>
		<p><b>E-mail</b>&#160; <xsl:value-of select="lemail/text()"/></p>
		<p><b>Телефоны</b>&#160; <xsl:value-of select="lphone/text()"/></p>
		<p><b>Адреса</b>&#160; <xsl:value-of select="laddr/text()"/></p>
		<h3>Документы</h3>
		<ul>
		<xsl:for-each select="doc_list/*">
			<li><xsl:value-of select="text()"/></li>
		</xsl:for-each>
		</ul>
	</xsl:template>
</xsl:stylesheet>