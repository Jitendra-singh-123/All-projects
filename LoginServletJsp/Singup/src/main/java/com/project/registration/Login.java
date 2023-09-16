package com.project.registration;

import java.io.IOException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;

import jakarta.servlet.RequestDispatcher;
import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import jakarta.servlet.http.HttpSession;

/**
 * Servlet implementation class Login
 */
@WebServlet("/login")
public class Login extends HttpServlet {
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		String uemail = request.getParameter("username");
		String upwd = request.getParameter("password");
		HttpSession session = request.getSession();
		String redirectURL = "login.jsp"; 
		if(uemail == null || uemail.equals("")) {
			session.setAttribute("status", "invalidEmail");
		}
		if( upwd == null || upwd.equals("")) {
			session.setAttribute("status", "invalidUpwd");
		}
		else {
		try {
			Class.forName("com.mysql.jdbc.Driver");
			Connection con = DriverManager.getConnection("jdbc:mysql://localhost:3306/sakila", "root", "root");
			PreparedStatement pst = con.prepareStatement("select * from users where email=? and password=?");
			pst.setString(1, uemail);
			pst.setString(2, upwd);
			
			ResultSet rs = pst.executeQuery();
			if(rs.next()) {
				session.setAttribute("name", rs.getString("name"));
				redirectURL = "index.jsp";
			}
			else {
				session.setAttribute("status", "failed");
				redirectURL = "login.jsp";
			}
		}
		catch(Exception e) {
			e.printStackTrace();
		}
	}
		response.sendRedirect(redirectURL);
	}

}
