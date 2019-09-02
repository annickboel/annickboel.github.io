package com.dekses.jersey.docker.demo;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.Consumes;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import com.dekses.jersey.docker.demo.Credentials;
//https://stackoverflow.com/questions/26777083/best-practice-for-rest-token-based-authentication-with-jax-rs-and-jersey
//https://developer.okta.com/blog/2018/10/31/jwts-with-java
@Path("/auth")
public class AuthenticationResource {	
    @POST
    @Produces(MediaType.APPLICATION_JSON)
    @Consumes(MediaType.APPLICATION_JSON)
    @Path("/login")
    public Response login(Credentials credentials) {
        try {
	
            // Authenticate the user using the credentials provided
            authenticate(credentials.getEmail(), credentials.getPassword());

            // Issue a token for the user
            String token = issueToken(credentials.getEmail());

            // Return the token on the response
            return Response.ok(token).build();

        } catch (Exception e) {
            return Response.status(Response.Status.FORBIDDEN).build();
        }      
    }


    @POST
    @Produces(MediaType.APPLICATION_JSON)
    @Consumes(MediaType.APPLICATION_JSON)
    @Path("/register")
    public Response register(UserProfile userProfile) {
        try {
	    //TODO : Save userProfile in dataBase
	
            // Authenticate the user using the credentials provided
            authenticate(userProfile.getEmail(), userProfile.getPassword());

            // Issue a token for the user
            String token = issueToken(credentials.getEmail());

            // Return the token on the response
            return Response.ok(token).build();

        } catch (Exception e) {
            return Response.status(Response.Status.FORBIDDEN).build();
        }      
    }

    private void authenticate(String username, String password) throws Exception {
        // Authenticate against a database, LDAP, file or whatever
        // Throw an Exception if the credentials are invalid
    }

    private String issueToken(String username) {
        // Issue a token (can be a random String persisted to a database or a JWT token)
        // The issued token must be associated to a user
        // Return the issued token
        return "azerty";
    }
	
}
