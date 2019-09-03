import React, { Component } from 'react';
import { View, Text, StyleSheet, Image } from 'react-native';
import { Icon } from 'react-native-elements'


export default class LoginForm extends Component {

  render() {
    return (
      <View style={styles.container}>
      	<View style={styles.logo}>
      		<Image style={styles.image} source={require('../../assets/logo.png')}/>
	        <Text style={styles.text}>Trash</Text>
        </View>
      </View>
    );
  }
}

const styles = StyleSheet.create({
	container: {
		width: '100%',
	},
	logo: {
		marginTop: 20,
		justifyContent: 'center',
		alignItems: 'center'
	},
	image:{
		width: 80, 
		height: 80,
	},
	text: {
		fontSize: 30,
		fontWeight: 'normal',
		color: 'rgb(177, 198, 52)'
	} 
});
