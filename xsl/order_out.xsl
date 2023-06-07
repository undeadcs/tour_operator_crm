<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:output method="html" indent="yes" doctype-public="-//W3C//DTD HTML 4.0 Transitional//EN"/>
	<xsl:template match="morph">
		<h1>Заявка (<xsl:value-of select="id/text()"/>)</h1>
		<p><b>Создан</b>&#160; <xsl:value-of select="substring(created/text(),0,10)"/></p>
		<p><b>Ответственный</b>&#160; <xsl:value-of select="manager_txt/text()"/></p>
		<p><b>Статус</b>&#160; <xsl:choose>
			<xsl:when test="status/text()='1'">Открыта</xsl:when>
			<xsl:when test="status/text()='2'">Закрыта</xsl:when>
			<xsl:when test="status/text()='3'">Требует внимания</xsl:when>
			<xsl:otherwise>неизвестное</xsl:otherwise>
		</xsl:choose></p>
		<p><b>Состояние</b>&#160; <xsl:value-of select="state_txt/text()"/></p>
		<p><b>Направление</b>&#160; <xsl:value-of select="direct_txt/text()"/></p>
		<p><b>Количество человек</b>&#160; <xsl:value-of select="amount/text()"/></p>
		<p><b>Заказчик</b>&#160; (<b><xsl:choose>
			<xsl:when test="emptype/text()='0'">физ. лицо</xsl:when>
			<xsl:when test="emptype/text()='1'">юр. лицо</xsl:when>
			<xsl:otherwise>Не понять</xsl:otherwise>
		</xsl:choose></b>)&#160; <xsl:value-of select="employer_txt/text()"/></p>
		<p><b>Желаемая дата отъезда</b>&#160; <xsl:value-of select="substring(date1/text(),0,10)"/></p>
		<p><b>Желаемая дата приезда</b>&#160; <xsl:value-of select="substring(date2/text(),0,10)"/></p>
		<p><b>Комментарий</b>&#160; <xsl:value-of select="comment/text()"/></p>
		<h3>Маршрут</h3>
		<ul>
		<xsl:for-each select="route_list/*">
			<li><xsl:value-of select="name/text()"/></li>
		</xsl:for-each>
		</ul>
		<h3>Услуги</h3>
		<ul>
		<xsl:for-each select="service_list/*">
			<li><xsl:value-of select="text()"/></li>
		</xsl:for-each>
		</ul>
	</xsl:template>
</xsl:stylesheet>