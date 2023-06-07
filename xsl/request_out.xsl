<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:output method="html" indent="yes" doctype-public="-//W3C//DTD HTML 4.0 Transitional//EN"/>
	<xsl:template match="morph">
		<h1>Обращение (<xsl:value-of select="id/text()"/>)</h1>
		<p><b>Создано</b>&#160; <xsl:value-of select="substring(created/text(),0,10)"/></p>
		<p><b>Ответственный</b>&#160; <xsl:value-of select="manager_txt/text()"/></p>
		<p><b>Статус</b>&#160; <xsl:choose>
			<xsl:when test="status/text()='1'">Открыто</xsl:when>
			<xsl:when test="status/text()='2'">Закрыто</xsl:when>
			<xsl:when test="status/text()='3'">Требует внимания</xsl:when>
			<xsl:otherwise>неизвестное</xsl:otherwise>
		</xsl:choose></p>
		<p><b>Состояние</b>&#160; <xsl:value-of select="state_txt/text()"/></p>
		<p><b>Тип</b>&#160; <xsl:value-of select="type_txt/text()"/></p>
		<p><b>Результат</b>&#160; <xsl:value-of select="result_txt/text()"/></p>
		<p><b>Направление</b>&#160; <xsl:value-of select="direct_txt/text()"/></p>
		<p><b>ФИО абонента</b>&#160; <xsl:value-of select="fio/text()"/></p>
		<p><b>Контактные данные</b>&#160; <xsl:value-of select="contact/text()"/></p>
		<p><b>Комментарий</b>&#160; <xsl:value-of select="comment/text()"/></p>
	</xsl:template>
</xsl:stylesheet>