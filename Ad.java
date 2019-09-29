package com.example.models;

import java.util.Date;
import java.math.BigDecimal;

public class Ad {

	Integer id;
	Integer userId;
	Integer categoryId;
	String description;
	BigDecimal price;
	Date created_at; 
	Date modified_at; 
	
	public Integer getId() {
		return id;
	}
	public void setId(Integer id) {
		this.id = id;
	}
	public Integer getUserId() {
		return userId;
	}
	public void setUserId(Integer userId) {
		this.userId = userId;
	}
	public Integer getCategoryId() {
		return categoryId;
	}
	public void setCategoryId(Integer categoryId) {
		this.categoryId = categoryId;
	}
	public String getDescription() {
		return description;
	}
	public void setDescription(String description) {
		this.description = description;
	}
	public Date getCreatedAt() {
		return created_at;
	}
	public void setPrice(BigDecimal price) {
		this.price = price;
	}
	public BigDecimal getPrice() {
		return price;
	}
	public void setCreatedAt(Date created_at) {
		this.created_at = created_at;
	}
	public Date getModifiedAt() {
		return modified_at;
	}
	public void setModifiedAt(Date modified_at) {
		this.modified_at = modified_at;
	}
}