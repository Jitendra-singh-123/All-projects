package com.project.registration;

import java.io.IOException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;

import jakarta.servlet.RequestDispatcher;
import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import jakarta.servlet.http.HttpSession;
@WebServlet("/newPassword")
public class NewPassword extends HttpServlet {
    private static final long serialVersionUID = 1L;

    protected void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {

        HttpSession session = request.getSession();
        String newPassword = request.getParameter("password");
        String confPassword = request.getParameter("confPassword");
        String redirectURL = "login.jsp";

        if (newPassword != null && confPassword != null && newPassword.equals(confPassword)) {

            try {
                Class.forName("com.mysql.jdbc.Driver");
                Connection con = DriverManager.getConnection("jdbc:mysql://localhost:3306/sakila", "root", "root");
                PreparedStatement pst = con.prepareStatement("UPDATE users SET password = ? WHERE email = ?");
                pst.setString(1, newPassword);
                pst.setString(2, (String) session.getAttribute("email"));

                int rowCount = pst.executeUpdate();
                if (rowCount > 0) {
                    session.setAttribute("status", "resetSuccess");
                    response.sendRedirect(redirectURL);
                } else {
                    session.setAttribute("status", "resetFailed");
                    response.sendRedirect(redirectURL);
                }

            } 
            catch (Exception e) {
                e.printStackTrace();
            }
        }
        else {
            session.setAttribute("status", "resetFailed");
            response.sendRedirect(redirectURL);
        }
    }
}
