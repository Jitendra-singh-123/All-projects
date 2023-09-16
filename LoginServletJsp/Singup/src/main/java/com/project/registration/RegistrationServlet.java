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

/**
 * Servlet implementation class RegistrationServlet
 */
@WebServlet("/register")
public class RegistrationServlet extends HttpServlet {
	
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
		//PrintWriter out = response.getWriter();
		String uname = request.getParameter("name");
		String uemail = request.getParameter("email");
		String upwd = request.getParameter("pass");
		String umobile = request.getParameter("contact");
		String Repwd = request.getParameter("re_pass");
		RequestDispatcher rd = null;
		
		if(uname == null || uname.equals("")) {
			request.setAttribute("status", "invalidName");
			 rd = request.getRequestDispatcher("registration.jsp");
			rd.forward(request, response);
		}
		if( uemail == null || uemail.equals("")) {
			request.setAttribute("status", "invalidEmail");
			rd = request.getRequestDispatcher("registration.jsp");
			rd.forward(request, response);
		}
		if(upwd == null || upwd.equals("")) {
			request.setAttribute("status", "invalidUpwd");
			rd = request.getRequestDispatcher("registration.jsp");
			rd.forward(request, response);
		}
		else if(!upwd.equals(Repwd)) {
			request.setAttribute("status", "invalidConfirmPassword");
			rd = request.getRequestDispatcher("registration.jsp");
			rd.forward(request, response);
		}
		if( umobile == null || umobile.equals("")) {
			request.setAttribute("status", "invalidMobile");
			rd = request.getRequestDispatcher("registration.jsp");
			rd.forward(request, response);
		}
		
		else if(umobile.length() < 10) {
			request.setAttribute("status", "InvaidMobileLength");
			rd = request.getRequestDispatcher("registration.jsp");
			rd.forward(request, response);
		}
		
		try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            Connection con = DriverManager.getConnection("jdbc:mysql://localhost:3306/sakila?useSSL=false", "root",
                    "root");
            
            // Check if the user already exists in the database
            PreparedStatement pst = con.prepareStatement("SELECT * FROM users WHERE email = ?");
            pst.setString(1, uemail);
            ResultSet result = pst.executeQuery();
            
            if (result.next()) {
                // User already exists
                request.setAttribute("status", "userAlreadyExists");
                rd = request.getRequestDispatcher("registration.jsp");
                rd.forward(request, response);
            } else {
                // User does not exist, proceed with registration
                PreparedStatement pst1 = con.prepareStatement(
                        "INSERT INTO users(name, email, password, umobile) VALUES(?,?,?,?)");
                pst1.setString(1, uname);
                pst1.setString(2, uemail);
                pst1.setString(3, upwd);
                pst1.setString(4, umobile);
                int row = pst1.executeUpdate();

                if (row > 0) {
                    request.setAttribute("status", "success");
                    rd = request.getRequestDispatcher("registration.jsp");
                    rd.forward(request, response);
                } else {
                    request.setAttribute("status", "failed");
                    rd = request.getRequestDispatcher("registration.jsp");
                    rd.forward(request, response);
                }
            }
        } 
		catch(Exception e) {
			e.printStackTrace();
		}
	
		
	}

}
